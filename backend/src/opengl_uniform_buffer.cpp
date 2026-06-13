#include "glad/glad.h"
#include <iostream>

#include "opengl_uniform_buffer.hpp"

std::shared_ptr<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding){
    return std::make_shared<opengl_uniform_buffer>(size, binding);
}

opengl_uniform_buffer::opengl_uniform_buffer(uint32_t size, uint32_t binding){
    glCreateBuffers(1, &_id);
    glNamedBufferStorage(_id, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, _id);
}

void opengl_uniform_buffer::setData(const void* data, uint32_t size, uint32_t offset){
    glNamedBufferSubData(_id, offset, size, data);
}

opengl_uniform_buffer::~opengl_uniform_buffer(){
    glDeleteBuffers(1, &_id);
}
