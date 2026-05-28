#pragma once

#include "renderer/buffer_layout.hpp"
#include "glad/glad.h"

class opengl_vertex_buffer : public VertexBuffer{
public:
    opengl_vertex_buffer(float* vertices, uint32_t size, BufferUsageType utype);
    ~opengl_vertex_buffer();

    void bind() const override;
    void unbind() const override;

    void setData(const void* data, uint32_t size) override;
    const BufferLayout& getLayout() const override;
    void setLayout(const BufferLayout& layout) override;

private:
    BufferLayout _layout;
    uint32_t _vbo;
    BufferUsageType _utype;
};

class opengl_element_buffer : public ElementBuffer{
public:
    opengl_element_buffer(uint32_t* indices, uint32_t count, BufferUsageType utype);
    ~opengl_element_buffer();

    uint32_t getCount() const override;

    void bind() const override;
    void unbind() const override;
private:
    uint32_t _size;
    uint32_t _ebo;
    BufferUsageType _utype;
};
