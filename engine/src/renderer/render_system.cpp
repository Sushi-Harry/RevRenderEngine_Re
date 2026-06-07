#include "renderer/renderer.hpp"
#include "renderer/render_system.hpp"
#include "renderer/render_calls.hpp"

void RenderSystem::BeginFrame(){
    _render_queue.clear();
}

void RenderSystem::Submit(const RenderCall& _render_packet, ResourceManager& res_mgr){
    // COMMENTING THIS OUT CAUSE I THINK THE TEXTURE SLOT UNIFORMS SHOULD BE SET ONCE EVERY SINGLE FRAME
    // /\
    ///  \
    // ||
    // ||

    // auto shader = res_mgr.get_shader(_render_packet._shader_id);
    // shader->bindShader();
    // const Material& material = res_mgr.get_material(_render_packet._material_id);

    // uint32_t texture_slot = 0;

    // for(size_t i = 0; i < material._diffuse_textures.size(); i++){
    //     auto tex = res_mgr.get_texture(material._diffuse_textures[i]);
    //     tex->bind(texture_slot);

    //     std::string uniform_name = "u_TextureDiffuse" + std::to_string(i);
    //     shader->setInt(uniform_name, texture_slot);

    //     texture_slot++;
    // }

    // for(size_t i = 0; i < material._specular_textures.size(); i++){
    //     auto tex = res_mgr.get_texture(material._specular_textures[i]);
    //     tex->bind(texture_slot);
    //     std::string uniform_name = "u_TextureSpecular" + std::to_string(i);
    //     shader->setInt(uniform_name, texture_slot);
    //     texture_slot++;
    // }
    _render_queue.push_back(_render_packet);
}

void RenderSystem::EndFrame(const ResourceManager& res_mgr, const glm::mat4& view_proj_mat){
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

    // WORK IN PROGRESS HERE
    // /\
    ///  \
    // ||
    // ||
    // Executing the drawing stuff
    uint32_t current_shader_id = std::numeric_limits<uint32_t>::max();
    uint32_t current_material_id = std::numeric_limits<uint32_t>::max();
    for(const auto& call : _render_queue){
        // Binding the shader
        if(call._shader_id != current_shader_id){
            std::shared_ptr<Shader> shader = res_mgr.get_shader(call._shader_id);
            shader->bindShader();

            // Camera math only needs to be sent once per shader
            shader->setMat4("u_ViewProjection", view_proj_mat);
            current_shader_id = call._shader_id;
        }
        std::shared_ptr<Shader> activeShader = res_mgr.get_shader(call._shader_id);

        // Now setting the material data in the shader if material data exists
        if(call._material_id != current_material_id){
            const Material& mat = res_mgr.get_material(call._material_id);
            int texture_slot = 0;

            if(!mat._diffuse_textures.empty() || !mat._specular_textures.empty()){

                // Diffuse textures
                for(int i = 0; i < mat._diffuse_textures.size(); i++){
                    auto tex = res_mgr.get_texture(mat._diffuse_textures[i]);
                    tex->bind(texture_slot);
                    activeShader->setInt("u_TextureDiffuse" + std::to_string(i), texture_slot);
                    texture_slot++; // Fixed increment!
                }

                // Specular textures
                for(int i = 0; i < mat._specular_textures.size(); i++){
                    auto tex = res_mgr.get_texture(mat._specular_textures[i]);
                    tex->bind(texture_slot);
                    activeShader->setInt("u_TextureSpecular" + std::to_string(i), texture_slot);
                    texture_slot++; // Fixed increment!
                }
            }
            current_material_id = call._material_id;
        }
        // Uploading the model matrix and other stuff
        activeShader->setMat4("u_ModelMatrix", call._model_matrix);
        DrawCommands::DrawIndexed(call._vao, call._idx_count);
    }
}
