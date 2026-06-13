#pragma once

#include <memory>

class UniformBuffer{
public:
    virtual ~UniformBuffer() = default;
    virtual void setData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

    static std::shared_ptr<UniformBuffer> Create(uint32_t size, uint32_t binding);
};
