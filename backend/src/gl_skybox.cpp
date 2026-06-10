#include "gl_skybox.hpp"

void gl_skybox::init(ResourceManager& res_mgr){
    float vertices[] = {
        // Unique 8 corners of a unit cube
        -1.0f,  1.0f, -1.0f, // 0
        -1.0f, -1.0f, -1.0f, // 1
         1.0f, -1.0f, -1.0f, // 2
         1.0f,  1.0f, -1.0f, // 3
        -1.0f,  1.0f,  1.0f, // 4
        -1.0f, -1.0f,  1.0f, // 5
         1.0f, -1.0f,  1.0f, // 6
         1.0f,  1.0f,  1.0f  // 7
    };

    uint32_t indices[] = {
        // Back
        1, 0, 3, 3, 2, 1,
        // Left
        5, 4, 0, 0, 1, 5,
        // Right
        2, 3, 7, 7, 6, 2,
        // Front
        5, 6, 7, 7, 4, 5,
        // Top
        4, 7, 3, 3, 0, 4,
        // Bottom
        1, 2, 6, 6, 5, 1
    };

    auto vbo = VertexBuffer::Create(vertices, sizeof(vertices), BufferUsageType::STATIC);
    _vao = VertexArray::Create();
    BufferLayout layout = {
        {"aPosition", ShaderDataType::FLOAT3}
    };
    vbo->setLayout(layout);
    _vao->addVertexBuffer(vbo);

    auto ebo = ElementBuffer::Create(indices, 36, BufferUsageType::STATIC);
    _vao->setElementBuffer(ebo);
}
