#include "opengl_buffer.hpp"

// The static create functions from the abstract classes
std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size, BufferUsageType utype){
    return std::make_shared<opengl_vertex_buffer>(vertices, size, utype);
}

std::shared_ptr<ElementBuffer> ElementBuffer::Create(uint32_t* indices, uint32_t count, BufferUsageType utype){
    return std::make_shared<opengl_element_buffer>(indices, count, utype);
}

// Opengl backend vertex buffer implementation
opengl_vertex_buffer::opengl_vertex_buffer(float* vertices, uint32_t size, BufferUsageType utype) : _utype(utype){
    // glCreateBuffers(1, &_vbo);
    // Just found out that glCreateBuffers does the glGenBuffers job but also initializes the buffer obj state
    // But since the buffer obj state is initialized whenever I first bind the buffer, I don't need glCreateBuffers. glGenBuffers with do just fine
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    if(utype == BufferUsageType::STATIC) {
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    } else if(utype == BufferUsageType::DYNAMIC){
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
    }else{
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STREAM_DRAW);
    }
}

void opengl_vertex_buffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
}

void opengl_vertex_buffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void opengl_vertex_buffer::setData(const void* data, uint32_t size){
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

const BufferLayout& opengl_vertex_buffer::getLayout() const { return _layout; }
void opengl_vertex_buffer::setLayout(const BufferLayout& layout) { _layout = layout; }

opengl_vertex_buffer::~opengl_vertex_buffer(){
    glDeleteBuffers(1, &_vbo);
}

opengl_element_buffer::opengl_element_buffer(uint32_t* indices, uint32_t count, BufferUsageType utype) : _size(count), _utype(utype) {
    // glCreateBuffers(1, &_ebo);
    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    if(utype == BufferUsageType::STATIC){
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }else if(utype == BufferUsageType::DYNAMIC){
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(uint32_t), indices, GL_DYNAMIC_DRAW);
    }else{
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(uint32_t), indices, GL_STREAM_DRAW);
    }
}

void opengl_element_buffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
}

void opengl_element_buffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t opengl_element_buffer::getCount() const { return _size; }

opengl_element_buffer::~opengl_element_buffer(){
    glDeleteBuffers(1, &_ebo);
}
