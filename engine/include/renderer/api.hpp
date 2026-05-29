#pragma once

#include <cstdint>
#include "glm/glm.hpp"
#include "renderer/vertex_array.hpp"

class RenderingAPI{
public:
    virtual ~RenderingAPI() = default;

    virtual void Init() = 0;
    virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void clearColor(const glm::vec4& color) = 0;
    virtual void clear() = 0;

    virtual void drawElements(const std::shared_ptr<VertexArray>& vert_array, uint32_t idx_cnt = 0) = 0;
    virtual void drawLines(const std::shared_ptr<VertexArray>& vert_array, uint32_t vert_cnt) = 0;

    virtual void setLineWidth(float width) = 0;

    static std::unique_ptr<RenderingAPI> create();
};
