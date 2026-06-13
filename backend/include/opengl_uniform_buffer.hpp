#pragma once

#include "renderer/uniform_buffer.hpp"

class opengl_uniform_buffer : public UniformBuffer{
public:
    opengl_uniform_buffer(uint32_t size, uint32_t binding);
    virtual ~opengl_uniform_buffer();

    void setData(const void* data, uint32_t size, uint32_t offset = 0) override;
private:
    uint32_t _id = 0;
};
