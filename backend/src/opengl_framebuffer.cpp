#include "opengl_framebuffer.hpp"
#include "glad/glad.h"
#include <iostream>
#include "core/application.hpp"

std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecs &specs){
    return std::make_shared<opengl_framebuffer>(specs);
}

opengl_framebuffer::opengl_framebuffer(const FramebufferSpecs& specs) : _fb_specs(specs){
    init();
}

opengl_framebuffer::~opengl_framebuffer(){
    release();
}

void opengl_framebuffer::release(){
    if(_id){
        glDeleteFramebuffers(1, &_id);
        if(!_color_attachments.empty()){
            glDeleteTextures(_color_attachments.size(), _color_attachments.data());
        }
        if(_depth_attachment){
            glDeleteTextures(1, &_depth_attachment);
        }

        _color_attachments.clear();
        _depth_attachment = 0;
    }
}

void opengl_framebuffer::init(){
    release();

    glCreateFramebuffers(1, &_id);

    bool hasColor = false;
    for(auto format : _fb_specs._attachments){
        if(format == FramebufferTextureFormat::REV_FB_RGBA8){
            uint32_t color_tex;
            glCreateTextures(GL_TEXTURE_2D, 1, &color_tex);
            glTextureStorage2D(color_tex, 1, GL_RGBA8, _fb_specs._width, _fb_specs._height);

            glTextureParameteri(color_tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(color_tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            uint32_t index = _color_attachments.size();
            glNamedFramebufferTexture(_id, GL_COLOR_ATTACHMENT0 + index, color_tex, 0);
            _color_attachments.push_back(color_tex);
            hasColor = true;
        }else if(format == FramebufferTextureFormat::REV_FB_DEPTH32F){
            glCreateTextures(GL_TEXTURE_2D, 1, &_depth_attachment);
            glTextureStorage2D(_depth_attachment, 1, GL_DEPTH_COMPONENT32F, _fb_specs._width, _fb_specs._height);

            glTextureParameteri(_depth_attachment, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTextureParameteri(_depth_attachment, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTextureParameteri(_depth_attachment, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTextureParameteri(_depth_attachment, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

            float border_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
            glTextureParameterfv(_depth_attachment, GL_TEXTURE_BORDER_COLOR, border_color);

            glNamedFramebufferTexture(_id, GL_DEPTH_ATTACHMENT, _depth_attachment, 0);
        }else if(format == FramebufferTextureFormat::REV_FB_INT32){
            uint32_t color_tex;
            glCreateTextures(GL_TEXTURE_2D, 1, &color_tex);
            glTextureStorage2D(color_tex, 1, GL_R32I, _fb_specs._width, _fb_specs._height);

            // Never knew this one. R32I type attachments must always use GL_NEAREST type filtering
            glTextureParameteri(color_tex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTextureParameteri(color_tex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            uint32_t index = _color_attachments.size();
            glNamedFramebufferTexture(_id, GL_COLOR_ATTACHMENT0 + index, color_tex, 0);
            _color_attachments.push_back(color_tex);
            hasColor = true;
        }else if (format == FramebufferTextureFormat::REV_FB_RGBA16F) {
            uint32_t color_tex;
            glCreateTextures(GL_TEXTURE_2D, 1, &color_tex);
            glTextureStorage2D(color_tex, 1, GL_RGBA16F, _fb_specs._width, _fb_specs._height);

            glTextureParameteri(color_tex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTextureParameteri(color_tex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            uint32_t index = _color_attachments.size();
            glNamedFramebufferTexture(_id, GL_COLOR_ATTACHMENT0 + index, color_tex, 0);
            _color_attachments.push_back(color_tex);
            hasColor = true;
        }else if(format == FramebufferTextureFormat::REV_FB_RGBA32F){
            uint32_t color_tex;
            glCreateTextures(GL_TEXTURE_2D, 1, &color_tex);
            glTextureStorage2D(color_tex, 1, GL_RGBA32F, _fb_specs._width, _fb_specs._height);

            glTextureParameteri(color_tex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTextureParameteri(color_tex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            uint32_t index = _color_attachments.size();
            glNamedFramebufferTexture(_id, GL_COLOR_ATTACHMENT0 + index, color_tex, 0);
            _color_attachments.push_back(color_tex);
            hasColor = true;
        }
    }

    if(!hasColor){
        glNamedFramebufferDrawBuffer(_id, GL_NONE);
        glNamedFramebufferReadBuffer(_id, GL_NONE);
    }

    if(glCheckNamedFramebufferStatus(_id, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cerr << "ERROR::OPENGL_FRAMEBUFFER::INCOMPLETE_FRAMEBUFFER\n";
    }

    // I SWEAR TO GOD I HATE HOW MUCH TROUBLE NOT ADDING THIS CODE GAVE ME.
    // I HAD NO GOD DAMN IDEA I WAS SUPPOSED TO EXPLICITLY TELL OPENGL EVERYTHING IT HAD TO DO WHEN THERE WERE MULTIPLE
    // ATTACHMENTS WHICH NEEDED DRAWING
    if(_color_attachments.size() > 1){
        std::vector<GLenum> drawBuffers;
        for (size_t i = 0; i < _color_attachments.size(); i++) {
            drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
        }

        glNamedFramebufferDrawBuffers(_id, static_cast<GLsizei>(drawBuffers.size()), drawBuffers.data());
    }else{
        GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
        glNamedFramebufferDrawBuffers(_id, 1, &drawBuffer);
    }
}

void opengl_framebuffer::bind(){
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    glViewport(0, 0, _fb_specs._width, _fb_specs._height);
}

void opengl_framebuffer::unbind(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Application::getInstance().getWindow().getWidth(), Application::getInstance().getWindow().getHeight());
}

void opengl_framebuffer::resize(uint32_t width, uint32_t height){
    _fb_specs._width = width;
    _fb_specs._height = height;
    init();
}

uint32_t opengl_framebuffer::get_color_attachment_id(uint32_t idx) const {
    if(idx >= _color_attachments.size() || idx < 0){
        std::cerr << "ERROR::OPENGL_FRAMEBUFFER::IDX_OUT_OF_BOUNDS\n";
        return 0;
    }
    return _color_attachments[idx];
}

uint32_t opengl_framebuffer::get_depth_attachment_id() const {
    return _depth_attachment;
}

const FramebufferSpecs& opengl_framebuffer::get_specifications() const {
    return _fb_specs;
}
