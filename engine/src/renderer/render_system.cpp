#include "renderer/renderer.hpp"
#include "renderer/render_system.hpp"
#include "renderer/render_calls.hpp"

void RenderSystem::BeginFrame(){
    _render_queue.clear();
}

void RenderSystem::Submit(const RenderCall& _render_packet){
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

    // Executing the drawing stuff
    uint32_t current_shader_id = std::numeric_limits<uint32_t>::max();
    uint32_t current_material_id = std::numeric_limits<uint32_t>::max();
    for(const auto& call : _render_queue){
        // Binding the shader
        if(call._shader_id != current_shader_id){
            std::shared_ptr<Shader> shader = res_mgr.get_shader(call._shader_id);
            shader->bindShader();
            // CAMERA MATH HERE!!!!!!!!!!!!1!!!!
            shader->setMat4("u_ViewProjection", view_proj_mat);
            current_shader_id = call._shader_id;
        }

        // Binding the material
        if(call._material_id != current_material_id){
            const Material& mat = res_mgr.get_material(call._material_id);
            if(!mat._diffuse_textures.empty()){
                std::shared_ptr<Texture2D> tex = res_mgr.get_texture(mat._diffuse_textures[0]);
                tex->bind(0);
            }
            current_material_id = call._material_id;
        }

        // Uploading the model matrix and other stuff
        std::shared_ptr<Shader> activeShader = res_mgr.get_shader(current_shader_id);
        activeShader->setMat4("u_ModelMatrix", call._model_matrix);
        DrawCommands::DrawIndexed(call._vao, call._idx_count);
    }

}
