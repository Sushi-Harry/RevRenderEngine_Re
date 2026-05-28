#pragma once

#include "glad/glad.h"
#include "renderer/vertex_array.hpp"

class opengl_vertex_array : public VertexArray{
public:
    opengl_vertex_array();
    virtual ~opengl_vertex_array();

    void bind() const override;
    void unbind() const override;

    void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vb) override;
    void setElementBuffer(const std::shared_ptr<ElementBuffer>& eb) override;

    const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const override;
    const std::shared_ptr<ElementBuffer>& getElementBuffer() const override;

private:
    uint32_t _vao;
    uint32_t _vb_idx = 0; // vertex buffer index
    std::vector<std::shared_ptr<VertexBuffer>> _vertex_buffers;
    std::shared_ptr<ElementBuffer> _element_buffer;
};
