#include "imgui.h"
#include "renderer/postprocessing.hpp"
#include "glad/glad.h"
#include "renderer/render_calls.hpp"

void ChromaticAbberation::onRenderGUI(){
    ImGui::Checkbox("Enabled", &_is_enabled);
    if(_is_enabled){
        ImGui::SliderFloat("Intensity", &_intensity, 0.0F, 0.05F, "%.4F");
    }
}

void ChromaticAbberation::Render(ResourceManager& res_mgr, uint32_t source_texture_id, std::shared_ptr<Framebuffer> dest_fbo){
    dest_fbo->bind();
        auto shader = res_mgr.get_shader("chromatic_aberration");
        shader->bindShader();
        shader->setFloat("u_Intensity", _intensity);
        glBindTextureUnit(0, source_texture_id);
        GeneralRenderCalls::draw_full_screen_quad();
    dest_fbo->unbind();
}
