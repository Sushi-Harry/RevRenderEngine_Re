#include "imgui.h"
#include "renderer/postprocessing.hpp"
#include "glad/glad.h"
#include "renderer/render_calls.hpp"

void PostProcessingSystem::Init(uint32_t width, uint32_t height){
    FramebufferSpecs specs = {};
    specs._attachments = { FramebufferTextureFormat::REV_FB_RGBA8 };
    specs._width = width;
    specs._height = height;
    _ping_pong_fbos[0] = Framebuffer::Create(specs);
    _ping_pong_fbos[1] = Framebuffer::Create(specs);
}

void PostProcessingSystem::Resize(uint32_t width, uint32_t height){
    _ping_pong_fbos[0]->resize(width, height);
    _ping_pong_fbos[1]->resize(width, height);
}

uint32_t PostProcessingSystem::Execute(uint32_t initial_src_texture, ResourceManager& res_mgr, const PostProcessingContext& ctx){
    uint32_t currentSrc = initial_src_texture;
    bool horizontal = true;
    for(const auto& effect : _effects){
        if(!effect->isEnabled()){
            continue;
        }

        effect->Render(res_mgr, currentSrc, _ping_pong_fbos[horizontal], ctx);

        currentSrc = _ping_pong_fbos[horizontal]->get_color_attachment_id();
        horizontal = !horizontal;
    }
    return currentSrc;
}

void ChromaticAbberation::onRenderGUI(){
    ImGui::Checkbox("Enabled", &_is_enabled);
    if(_is_enabled){
        ImGui::SliderFloat("Intensity", &_intensity, 0.0F, 0.05F, "%.4F");
    }
}

void ChromaticAbberation::Render(ResourceManager& res_mgr, uint32_t source_texture_id, std::shared_ptr<Framebuffer> dest_fbo, const PostProcessingContext& ctx){
    dest_fbo->bind();
        auto shader = res_mgr.get_shader("chromatic_aberration");
        shader->bindShader();
        shader->setFloat("u_Intensity", _intensity);
        glBindTextureUnit(0, source_texture_id);
        shader->setInt("u_SceneTexture", 0);
        GeneralRenderCalls::draw_full_screen_quad();
    dest_fbo->unbind();
}

void Pixelation::onRenderGUI(){
    ImGui::Checkbox("Enabled", &_is_enabled);
    if(_is_enabled){
        ImGui::SliderFloat("Block Size", &_block_size, 1.0F, 16.0F, "%.1f");
    }
}

void Pixelation::Render(ResourceManager& res_mgr, uint32_t source_texture_id, std::shared_ptr<Framebuffer> dest_fbo, const PostProcessingContext& ctx){
    dest_fbo->bind();
        auto shader = res_mgr.get_shader("pixelation");
        shader->bindShader();
        shader->setFloat("u_BlockSize", _block_size);
        shader->setVec2("u_Resolution", ctx._resolution);
        glBindTextureUnit(0, source_texture_id);
        shader->setInt("u_SceneTexture", 0);
        GeneralRenderCalls::draw_full_screen_quad();
    dest_fbo->unbind();
}


void CRT::onRenderGUI(){
    ImGui::Checkbox("Enabled", &_is_enabled);
    if(_is_enabled){
        ImGui::SliderFloat("Line Count", &_line_count, 100.0F, 2000.0F, "%.0f");
        ImGui::SliderFloat("Intensity", &_intensity, 0.0F, 1.0F, "%.1f");
    }
}

void CRT::Render(ResourceManager& res_mgr, uint32_t source_texture_id, std::shared_ptr<Framebuffer> dest_fbo, const PostProcessingContext& ctx){
    dest_fbo->bind();
        auto shader = res_mgr.get_shader("crt");
        shader->bindShader();
        shader->setFloat("u_Intensity", _intensity);
        shader->setFloat("u_LineCount", _line_count);
        glBindTextureUnit(0, source_texture_id);
        shader->setInt("u_SceneTexture", 0);
        GeneralRenderCalls::draw_full_screen_quad();
    dest_fbo->unbind();
}

void FilmGrain::onRenderGUI(){
    ImGui::Checkbox("Enabled", &_is_enabled);
    if(_is_enabled){
        ImGui::SliderFloat("Intensity", &_intensity, 0.0F, 1.0F, "%.1f");
    }
}

void FilmGrain::Render(ResourceManager& res_mgr, uint32_t source_texture_id, std::shared_ptr<Framebuffer> dest_fbo, const PostProcessingContext& ctx){
    dest_fbo->bind();
        auto shader = res_mgr.get_shader("crt");
        shader->bindShader();
        shader->setFloat("u_Intensity", _intensity);
        shader->setFloat("u_Time", ctx._time);
        glBindTextureUnit(0, source_texture_id);
        shader->setInt("u_SceneTexture", 0);
        GeneralRenderCalls::draw_full_screen_quad();
    dest_fbo->unbind();
}
