#include "opengl_texture.hpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::shared_ptr<Texture2D> Texture2D::Create(const TextureInfo& info){
    return std::make_shared<opengl_texture_2d>(info);
}

std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path){
    return std::make_shared<opengl_texture_2d>(path);
}

GLenum opengl_texture_2d::rev_format_to_gl_int_format(PixelFormat pf){
    switch (pf) {
        case PixelFormat::RGB8:
            return GL_RGB8;
        case PixelFormat::RGBA8:
            return GL_RGBA8;
    }
    return 0;
}
GLenum opengl_texture_2d::rev_format_to_gl_data_format(PixelFormat pf){
    switch (pf) {
        case PixelFormat::RGB8:
            return GL_RGB;
        case PixelFormat::RGBA8:
            return GL_RGBA;
    }
    return 0;
}

opengl_texture_2d::opengl_texture_2d(const std::string& path) : _path(path){
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = nullptr;
    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if(data){
        _is_loaded = true;
        _width = width;
        _height = height;

        GLenum int_format, data_format;
        if(nrChannels == 3){
            int_format = GL_RGB8;
            data_format = GL_RGB;
        }else if(nrChannels == 4){
            int_format = GL_RGBA8;
            data_format = GL_RGBA;
        }

        _internal_format = int_format;
        _data_format = data_format;

        glCreateTextures(GL_TEXTURE_2D, 1, &_texture_id);
        glTextureStorage2D(_texture_id, 1, _internal_format, _width, _height);

        glTextureParameteri(_texture_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(_texture_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(_texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(_texture_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if(_internal_format == GL_RGB8)
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        else if(_internal_format == GL_RGBA8)
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glTextureSubImage2D(_texture_id, 0, 0, 0, _width, _height, _data_format, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(_texture_id);
        stbi_image_free(data);
    }
}

opengl_texture_2d::opengl_texture_2d(const TextureInfo& info) : _info(info), _width(info._width), _height(info._height), _internal_format(rev_format_to_gl_int_format(_info._format)), _data_format(rev_format_to_gl_data_format(_info._format)){
    glCreateTextures(GL_TEXTURE_2D, 1, &_texture_id);
    glTextureStorage2D(_texture_id, 1, _internal_format, _width, _height);

    glTextureParameteri(_texture_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(_texture_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(_texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(_texture_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void opengl_texture_2d::setData(void* data, uint32_t size){
    uint32_t bpp = (_data_format == GL_RGB) ? 3 : 4;
    if(_internal_format == GL_RGB8)
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    else if(_internal_format == GL_RGBA8)
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTextureSubImage2D(_texture_id, 0, 0, 0, _width, _height, _data_format, GL_UNSIGNED_BYTE, data);
}

void opengl_texture_2d::bind(uint32_t slot) const {
    glBindTextureUnit(slot, _texture_id);
}

opengl_texture_2d::~opengl_texture_2d(){
    glDeleteTextures(1, &_texture_id);
}
