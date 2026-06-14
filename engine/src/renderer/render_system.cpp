#include "core/utilities.hpp"
#include "ecs/render_buffer_data.hpp"
#include "renderer/renderer.hpp"
#include "renderer/render_system.hpp"
#include "renderer/render_calls.hpp"
#include "renderer/material.hpp"

void RenderSystem::Init(){
    _camera_ubo = UniformBuffer::Create(sizeof(CameraBufferData), 0);
    _lights_ubo = UniformBuffer::Create(sizeof(LightBufferData), 1);
}

void RenderSystem::BeginFrame(const Camera3D& cam){
    _render_queue.clear();

    CameraBufferData cameraData;
    cameraData.Projection = cam.getProjectionMatrix();
    cameraData.ViewProjection = cam.getViewProjMatrix();
    cameraData.View = cam.getViewMatrix();
    cameraData.ViewPosition = cam.getViewPos();
    _camera_ubo->setData(&cameraData, sizeof(CameraBufferData));
}

void RenderSystem::Submit(const RenderCall& _render_packet, ResourceManager& res_mgr){
    _render_queue.push_back(_render_packet);
}

void RenderSystem::EndFrame(ResourceManager& res_mgr, const SceneData& scene_data){
    std::sort(_render_queue.begin(), _render_queue.end(), [](const RenderCall& a, const RenderCall& b){
        // So first we're gonna be sorting according to the shader
        if(a._shader_id != b._shader_id){
            return a._shader_id < b._shader_id;
        }
        // Next we're gonna sort by materials if the shaders are same
        if(a._material_id != b._material_id){
            return a._material_id < b._material_id;
        }
        // Now if both shader and material are same, we'll sort by the vao
        return a._vao < b._vao;
    });
    // Everything's been sorted now. We can now move on to setting uniforms.

    // This is for the directional light.
    LightBufferData lightData{};
    lightData._directional_light._direction = scene_data._directional_light._direction;
    lightData._directional_light._enabled = scene_data._directional_light._enabled ? 1 : 0; // lol
    lightData._directional_light._color = scene_data._directional_light._color;
    lightData._directional_light._ambient = scene_data._directional_light._ambient;
    lightData._directional_light._diffuse = scene_data._directional_light._diffuse;
    lightData._directional_light._specular = scene_data._directional_light._specular;
    lightData._directional_light._light_space_matrix = CalculateLightSpaceMatrix(scene_data._directional_light, scene_data._camera);

    // Now moving to the spot lights
    for(int i = 0; i < 4; i++){
        if(i < scene_data._spot_lights.size()) {
            lightData._spot_lights[i]._position     = scene_data._spot_lights[i]._position;
            lightData._spot_lights[i]._enabled      = scene_data._spot_lights[i]._enabled ? 1 : 0;
            lightData._spot_lights[i]._direction    = scene_data._spot_lights[i]._direction;
            lightData._spot_lights[i]._color        = scene_data._spot_lights[i]._color;
            lightData._spot_lights[i]._constant     = scene_data._spot_lights[i]._constant;
            lightData._spot_lights[i]._linear       = scene_data._spot_lights[i]._linear;
            lightData._spot_lights[i]._quadratic    = scene_data._spot_lights[i]._quadratic;
            lightData._spot_lights[i]._ambient      = scene_data._spot_lights[i]._ambient;
            lightData._spot_lights[i]._diffuse      = scene_data._spot_lights[i]._diffuse;
            lightData._spot_lights[i]._specular     = scene_data._spot_lights[i]._specular;

            // Pre-calculate cosines on the CPU so your fragment shader stays blazing fast!
            lightData._spot_lights[i]._inner_cutoff = glm::cos(glm::radians(scene_data._spot_lights[i]._inner_cutoff));
            lightData._spot_lights[i]._outer_cutoff = glm::cos(glm::radians(scene_data._spot_lights[i]._outer_cutoff));

            // Leave space matrix placeholder for spotlight shadow maps later
            lightData._spot_lights[i]._light_space_matrix = scene_data._spot_lights[i]._light_space_matrix;
        } else {
            lightData._spot_lights[i]._enabled = 0U;
        }
    }
    _lights_ubo->setData(&lightData, sizeof(LightBufferData));

    // Executing the drawing stuff
    uint32_t current_shader_id = std::numeric_limits<uint32_t>::max();
    uint32_t current_material_id = std::numeric_limits<uint32_t>::max();
    for(const auto& call : _render_queue){
        if(call._shader_id != current_shader_id){
            std::shared_ptr<Shader> activeShader = res_mgr.get_shader(call._shader_id);
            activeShader->bindShader();
            if(scene_data._directional_shadow_map_id){
                GeneralRenderCalls::bindTexture(scene_data._directional_shadow_map_id, TextureSlots::REV_SHADOWMAP_DIRECTIONAL);
                activeShader->setInt("u_ShadowMap", TextureSlots::REV_SHADOWMAP_DIRECTIONAL);
                // || ||    ========       ||===||
                // ||=||       ||          ||===<
                // || || ELD   || OGETHER  ||===|| Y

                // || ||         /\        ..__,
                // ||=||        /__\       ||__|
                // || || OPES  //  \\ ND   ||  RAYERS
            }
            if(scene_data._spot_shadow_map_id){
                GeneralRenderCalls::bindTexture(scene_data._spot_shadow_map_id, TextureSlots::REV_SHADOWMAP_SPOTLIGHT);
                activeShader->setInt("u_ShadowMaps_SpotLight", TextureSlots::REV_SHADOWMAP_SPOTLIGHT);
            }

            current_shader_id = call._shader_id;
            current_material_id = std::numeric_limits<uint32_t>::max();
        }
        std::shared_ptr<Shader> activeShader = res_mgr.get_shader(call._shader_id);

        // Now setting the material data in the shader if material data exists
        if(call._material_id != current_material_id){
            const Material& mat = res_mgr.get_material(call._material_id);
            mat.Apply(res_mgr, activeShader);
            current_material_id = call._material_id;
        }
        // Uploading the model matrix and other stuff
        activeShader->setMat4("u_ModelMatrix", call._model_matrix);
        DrawCommands::DrawIndexed(call._vao, call._idx_count);
    }
}

void RenderSystem::ShadowMappingRenderPass(ResourceManager& res_mgr, const glm::mat4& light_space_mat){
    auto shadow_map_shader =res_mgr.get_shader("shadow_map_shader");
    shadow_map_shader->bindShader();
    shadow_map_shader->setMat4("u_LightSpaceMatrix", light_space_mat);

    std::shared_ptr<VertexArray> current_vao = nullptr;
    for(const auto& call : _render_queue){
        if(call._vao != current_vao){
            call._vao->bind();
            current_vao = call._vao;
        }
        shadow_map_shader->setMat4("u_ModelMatrix", call._model_matrix);

        DrawCommands::DrawIndexed(call._idx_count);
    }
    DrawCommands::UnbindVAO();
}

glm::mat4 RenderSystem::CalculateLightSpaceMatrix(const DirectionalLightComponent& dirLight, const Camera3D& cam){
    glm::mat4 lightProj = glm::ortho(-10.0F, 10.0F, -10.0F, 10.0F, 0.1F, 50.0F);

    glm::vec3 lightPos = cam.getViewPos() - (glm::normalize(dirLight._direction) * 20.0F);
    glm::mat4 lightView = glm::lookAt(lightPos, cam.getViewPos(), glm::vec3(0.0f, 1.0f, 0.0f));

    return lightProj * lightView;
}

glm::mat4 RenderSystem::CalculateLightSpaceMatrix(const SpotLightComponent& sptLight, const Camera3D& cam){
    glm::mat4 lightProj = glm::perspective(glm::radians(90.0F), 1.0F, 1.0F, 50.0F);
    glm::vec3 upVector = glm::vec3(0.0F, 1.0F, 0.0F);

    // So when I tried to use this function for lights that were not completely antiparallel to the upVector, it would work
    // but without this part, shadows would completely vanish when the light source's direction was completely antiparallel. Why? Look at the devlog entry for 14 June, 2026
    if (glm::abs(sptLight._direction.y) > 0.999F) {
        upVector = glm::vec3(0.0F, 0.0F, 1.0F);
    }
    glm::mat4 lightView = glm::lookAt(sptLight._position, sptLight._position+sptLight._direction, upVector);
    glm::mat4 lightSpaceMat = lightProj * lightView;

    return lightSpaceMat;
}

void RenderSystem::ClearRenderQueue(){
    _render_queue.clear();
}
