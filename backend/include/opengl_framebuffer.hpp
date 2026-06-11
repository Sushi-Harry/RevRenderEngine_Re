#include "renderer/framebuffer.hpp"

class opengl_framebuffer : public Framebuffer{
public:
    opengl_framebuffer(const FramebufferSpecs& specs);
    ~opengl_framebuffer();

    void release();
    void init();

    void bind() override;
    void unbind() override;
    void resize(uint32_t width, uint32_t height) override;

    uint32_t get_color_attachment_id(uint32_t idx) const override;
    uint32_t get_depth_attachment_id() const override;

    const FramebufferSpecs& get_specifications() const override;
private:
    FramebufferSpecs _fb_specs;

    uint32_t _depth_attachment = 0;
    std::vector<uint32_t> _color_attachments;
    uint32_t _id = 0;
};
