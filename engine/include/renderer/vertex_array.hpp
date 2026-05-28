#pragma once

#include "renderer/buffer_layout.hpp"

class VertexArray{
public:
    VertexArray() = default;
    virtual ~VertexArray() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vb) = 0;
    virtual void setElementBuffer(const std::shared_ptr<ElementBuffer>& eb) = 0;

    virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const = 0;
    virtual const std::shared_ptr<ElementBuffer>& getElementBuffer() const = 0;

    static std::shared_ptr<VertexArray> Create();
};
