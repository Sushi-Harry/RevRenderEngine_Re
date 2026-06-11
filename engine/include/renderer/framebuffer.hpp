#pragma once
#include <cstdint>
#include <initializer_list>
#include <memory>
#include <vector>

enum class FramebufferTextureFormat : uint8_t{
    NONE,
    // Color attachments
    REV_FB_RGBA8,
    REV_FB_RED_INT,

    // Depth and stencil
    REV_FB_DEPTH24STENCIL8,
    REV_FB_DEPTH32F
};

struct FramebufferSpecs{
    uint32_t _width = 1280;
    uint32_t _height = 720;

    FramebufferSpecs() = default;
    FramebufferSpecs(std::initializer_list<FramebufferTextureFormat> tex_attchments) : _attachments(tex_attchments) {}

    std::vector<FramebufferTextureFormat> _attachments;
};

class Framebuffer{
public:
    virtual ~Framebuffer() = default;

    virtual void bind() = 0;
    virtual void unbind() = 0;
    virtual void resize(uint32_t width, uint32_t height) = 0;

    virtual uint32_t get_color_attachment_id(uint32_t idx = 0) const = 0;
    virtual uint32_t get_depth_attachment_id() const = 0;

    virtual const FramebufferSpecs& get_specifications() const = 0;

    static std::shared_ptr<Framebuffer> Create(const FramebufferSpecs& specs);
};
