#pragma once

#include "renderer/texture.hpp"
#include "glad/glad.h"

class opengl_texture_2d : public Texture2D{
public:
    opengl_texture_2d(const TextureInfo& info);
    opengl_texture_2d(const std::string& path);
    opengl_texture_2d(REV_TEXTURE_TYPE type);

    ~opengl_texture_2d();

    uint32_t getWidth() const override { return _width; }
    uint32_t getHeight() const override { return _height; }
    uint32_t getTextureID() const override { return _texture_id; }
    std::string getPath() const override { return _path; }
    const TextureInfo& getTextureInfo() const override { return _info; }

    void bind(uint32_t slot) const override;
    void setData(void* data, uint32_t size) override;
    bool isLoaded() const override { return _is_loaded; }

    static GLenum rev_format_to_gl_int_format(PixelFormat pf);
    static GLenum rev_format_to_gl_data_format(PixelFormat pf);
private:
    TextureInfo _info;
    bool _is_loaded = false;
    uint32_t _width, _height;
    uint32_t _texture_id;
    std::string _path;
    GLenum _internal_format, _data_format;
};
