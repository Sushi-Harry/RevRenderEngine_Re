#pragma once

#include <string>
#include <cstdint>
#include <memory>

enum class PixelFormat{
    RGB8,
    RGBA8
};

struct TextureInfo{
    uint32_t _width = 1;
    uint32_t _height = 1;
    PixelFormat _format = PixelFormat::RGB8;
    bool _gen_mipmaps = true;
};

class Texture {
public:
    virtual ~Texture() = default;

    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;
    virtual uint32_t getTextureID() const = 0;
    virtual std::string getPath() const = 0;
    virtual const TextureInfo& getTextureInfo() const = 0;

    virtual void bind(uint32_t slot) const = 0;
    virtual void setData(void* data, uint32_t size) = 0;
    virtual bool isLoaded() const = 0;
};

class Texture2D : public Texture{
public:
    static std::shared_ptr<Texture2D> Create(const TextureInfo& info);
    static std::shared_ptr<Texture2D> CreateDefault();
    static std::shared_ptr<Texture2D> Create(const std::string& path);
};
