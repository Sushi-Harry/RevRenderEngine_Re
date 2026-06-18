#pragma once

#include <cstdint>
#include "core/utilities.hpp"
#include "glm/glm.hpp"
#include "renderer/vertex_array.hpp"

class RenderingAPI{
public:
    virtual ~RenderingAPI() = default;

    virtual void Init() = 0;
    virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void clearColor(const glm::vec4& color) = 0;
    virtual void clear() = 0;
    virtual void clear_depth_buffer_bit_only() = 0;
    virtual void toggle_cursor_input_mode(CursorInputMode mode) = 0;

    virtual void drawElements(const std::shared_ptr<VertexArray>& vert_array, uint32_t idx_cnt = 0) = 0;
    virtual void drawLines(const std::shared_ptr<VertexArray>& vert_array, uint32_t vert_cnt) = 0;

    virtual void setLineWidth(float width) = 0;

    virtual void bindTexture(uint32_t _id, uint32_t slot) = 0;

    virtual void draw_full_screen_quad() = 0;

    virtual float get_time() = 0;

    static std::unique_ptr<RenderingAPI> create();
};
