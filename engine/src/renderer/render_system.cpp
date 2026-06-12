#include "renderer/renderer.hpp"
#include "renderer/render_system.hpp"
#include "renderer/render_calls.hpp"
#include "renderer/material.hpp"

void RenderSystem::BeginFrame(){
    _render_queue.clear();
}

void RenderSystem::Submit(const RenderCall& _render_packet, ResourceManager& res_mgr){
    _render_queue.push_back(_render_packet);
}

void RenderSystem::EndFrame(ResourceManager& res_mgr, const glm::mat4& view_proj_mat, const Camera3D& cam, const std::vector<PointLightComponent>& active_pl_data, const DirectionalLightComponent& dirLight, uint32_t depth_map_texture_directional){
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

    // Executing the drawing stuff
    uint32_t current_shader_id = std::numeric_limits<uint32_t>::max();
    uint32_t current_material_id = std::numeric_limits<uint32_t>::max();
    for(const auto& call : _render_queue){
        if(call._shader_id != current_shader_id){
            std::shared_ptr<Shader> activeShader = res_mgr.get_shader(call._shader_id);
            activeShader->bindShader();
            // Camera math only needs to be sent once per shader
            activeShader->setMat4("u_ViewProjection", view_proj_mat);
            activeShader->setVec3("u_ViewPos", cam.getViewPos());
            // ===============================================================================
            // || Setting Point Light Uniforms. (Might make a function for this code later) ||
            // ===============================================================================
            // Running the loop till 4 cause that's the maximum number of point lights allowed in the fragment sahder (Check default_lit_shader.frag)
            for(int i = 0; i < 4; i++){
                std::string _base = "u_PointLights[" + std::to_string(i) + "].";
                if(i < active_pl_data.size()){
                    activeShader->setBool(_base + "_enabled", active_pl_data[i]._enabled);

                    activeShader->setVec3(_base + "_position", active_pl_data[i]._position);
                    activeShader->setVec3(_base + "_color", active_pl_data[i]._color);

                    activeShader->setFloat(_base + "_constant", active_pl_data[i]._constant);
                    activeShader->setFloat(_base + "_linear", active_pl_data[i]._linear);
                    activeShader->setFloat(_base + "_quadratic", active_pl_data[i]._quadratic);

                    activeShader->setFloat(_base + "_ambient", active_pl_data[i]._ambient);
                    activeShader->setFloat(_base + "_diffuse", active_pl_data[i]._diffuse);
                    activeShader->setFloat(_base + "_specular", active_pl_data[i]._specular);
                }else{
                    activeShader->setBool(_base + "_enabled", false);
                }
            }
            // =====================================================================================
            // || Setting Directional Light Uniforms. (Might make a function for this code later) ||
            // =====================================================================================
            if(dirLight._enabled){
                std::string _base = "u_DirectionalLight.";
                activeShader->setBool(_base + "_enabled", dirLight._enabled);

                activeShader->setVec3(_base + "_color", dirLight._color);
                activeShader->setVec3(_base + "_direction", dirLight._direction);

                activeShader->setFloat(_base + "_ambient", dirLight._ambient);
                activeShader->setFloat(_base + "_diffuse", dirLight._diffuse);
                activeShader->setFloat(_base + "_specular", dirLight._specular);

                activeShader->setMat4("u_LightSpaceMatrix", CalculateLightSpaceMatrix(dirLight, cam));

                if(depth_map_texture_directional){
                    GeneralRenderCalls::bindTexture(depth_map_texture_directional, 2);
                    activeShader->setInt("u_ShadowMap", 2);
                }
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
