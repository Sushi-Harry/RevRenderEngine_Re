#pragma once
#include "renderer/api.hpp"
#include "glad/glad.h"

class opengl_renderer : public RenderingAPI{
public:
    opengl_renderer() = default;
    void Init() override;
    void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    void clearColor(const glm::vec4& color) override;
    void clear() override;

    void drawElements(const std::shared_ptr<VertexArray>& vert_array, uint32_t idx_cnt = 0) override;
    void drawLines(const std::shared_ptr<VertexArray>& vert_array, uint32_t vert_cnt) override;

    void setLineWidth(float width) override;

private:
};
