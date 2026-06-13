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

void RenderSystem::EndFrame(ResourceManager& res_mgr, const glm::mat4& view_proj_mat, const Camera3D& cam, const std::vector<SpotLightComponent>& active_pl_data, const DirectionalLightComponent& dirLight, uint32_t depth_map_texture_directional){
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
    lightData._directional_light._direction = dirLight._direction;
    lightData._directional_light._enabled = dirLight._enabled ? 1 : 0; // lol
    lightData._directional_light._color = dirLight._color;
    lightData._directional_light._ambient = dirLight._ambient;
    lightData._directional_light._diffuse = dirLight._diffuse;
    lightData._directional_light._specular = dirLight._specular;
    lightData._directional_light._light_space_matrix = CalculateLightSpaceMatrix(dirLight, cam);
    // Now moving to the spot lights
    for(int i = 0; i < 4; i++){
        if(i < active_pl_data.size()) {
            lightData._spot_lights[i]._position     = active_pl_data[i]._position;
            lightData._spot_lights[i]._enabled      = active_pl_data[i]._enabled ? 1 : 0;
            lightData._spot_lights[i]._direction    = active_pl_data[i]._direction;
            lightData._spot_lights[i]._color        = active_pl_data[i]._color;
            lightData._spot_lights[i]._constant     = active_pl_data[i]._constant;
            lightData._spot_lights[i]._linear       = active_pl_data[i]._linear;
            lightData._spot_lights[i]._quadratic    = active_pl_data[i]._quadratic;
            lightData._spot_lights[i]._ambient      = active_pl_data[i]._ambient;
            lightData._spot_lights[i]._diffuse      = active_pl_data[i]._diffuse;
            lightData._spot_lights[i]._specular     = active_pl_data[i]._specular;

            // Pre-calculate cosines on the CPU so your fragment shader stays blazing fast!
            lightData._spot_lights[i]._inner_cutoff = glm::cos(glm::radians(active_pl_data[i]._inner_cutoff));
            lightData._spot_lights[i]._outer_cutoff = glm::cos(glm::radians(active_pl_data[i]._outer_cutoff));

            // Leave space matrix placeholder for spotlight shadow maps later
            lightData._spot_lights[i]._light_space_matrix = glm::mat4(1.0f);
        } else {
            lightData._spot_lights[i]._enabled = false;
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
            if(depth_map_texture_directional){
                GeneralRenderCalls::bindTexture(depth_map_texture_directional, 2);
                activeShader->setInt("u_ShadowMap", 2);
                // || ||    ========       ||===||
                // ||=||       ||          ||===<
                // || || ELD   || OGETHER  ||===|| Y
                activeShader->setInt("u_ShadowMaps_SpotLight", 3);
                // || ||         /\        ..__,
                // ||=||        /__\       ||__|
                // || || OPES  //  \\ ND   ||  RAYERS
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

void RenderSystem::ClearRenderQueue(){
    _render_queue.clear();
}
