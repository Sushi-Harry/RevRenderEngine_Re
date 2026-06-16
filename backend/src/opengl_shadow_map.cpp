#include "core/application.hpp"
#include "glad/glad.h"

#include "opengl_shadow_map.hpp"

std::shared_ptr<ShadowMap> ShadowMap::Create(const ShadowMapSpecs &specs){
    return std::make_shared<opengl_shadow_map>(specs);
}

opengl_shadow_map::opengl_shadow_map(const ShadowMapSpecs& specs) : _specs(specs){
    AllocateResources();
}

opengl_shadow_map::~opengl_shadow_map(){
    glDeleteFramebuffers(1, &_framebuffer_id);
    glDeleteTextures(1, &_texture_id);
}

void opengl_shadow_map::AllocateResources() {
    switch (_specs._type) {

        case ShadowMapType::REV_SHADOW_MAP_SPOT:{
            glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &_texture_id);
            glTextureStorage3D(_texture_id, 1, GL_DEPTH_COMPONENT24, _specs._width, _specs._height, _specs._slices);
            break;
        }
        case ShadowMapType::REV_SHADOW_MAP_DIRECTIONAL:{
            glCreateTextures(GL_TEXTURE_2D, 1, &_texture_id);
            glTextureStorage2D(_texture_id, 1, GL_DEPTH_COMPONENT24, _specs._width, _specs._height);
            break;
        }
        default:{
            std::cout << "ERROR::SHADOW_MAP::UNKNOWN_OR_INVALID_TYPE\n";
            return;
        }
    }

    glTextureParameteri(_texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(_texture_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(_texture_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(_texture_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = {1.0F, 1.0F, 1.0F, 1.0F};
    glTextureParameterfv(_texture_id, GL_TEXTURE_BORDER_COLOR, borderColor);

    glCreateFramebuffers(1, &_framebuffer_id);

    if(_specs._type == ShadowMapType::REV_SHADOW_MAP_DIRECTIONAL){
        glNamedFramebufferTexture(_framebuffer_id, GL_DEPTH_ATTACHMENT, _texture_id, 0);
    }

    glNamedFramebufferDrawBuffer(_framebuffer_id, GL_NONE);
    glNamedFramebufferReadBuffer(_framebuffer_id, GL_NONE);
}

void opengl_shadow_map::BindLayerTarget(uint32_t layerIndex) const {
    glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _texture_id, 0, layerIndex);
}

void opengl_shadow_map::BindFramebuffer() const {
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer_id);
    glViewport(0, 0, _specs._width, _specs._height);
}

void opengl_shadow_map::UnbindFramebuffer() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    auto& window = Application::getInstance().getWindow();
    glViewport(0, 0, window.getWidth(), window.getHeight());
}

void opengl_shadow_map::BindTexture(uint32_t slot) const {
    glBindTextureUnit(slot, _texture_id);
}
