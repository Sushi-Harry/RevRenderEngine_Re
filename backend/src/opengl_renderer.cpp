#include "opengl_renderer.hpp"
#include "core/application.hpp"
#include <GLFW/glfw3.h>

std::unique_ptr<RenderingAPI> RenderingAPI::create(){
    return std::make_unique<opengl_renderer>();
}

void opengl_renderer::Init(){
    float quadVertices[] = {
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
        1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,   1.0f, 1.0f
    };

    uint32_t indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    _vao = VertexArray::Create();
    BufferLayout layout ={
        {"aPosition", ShaderDataType::FLOAT3},
        {"aTexCoords", ShaderDataType::FLOAT2}
    };
    auto _vbo = VertexBuffer::Create(quadVertices, sizeof(quadVertices), BufferUsageType::STATIC);
    _vbo->setLayout(layout);
    _vao->addVertexBuffer(_vbo);
    auto _ebo = ElementBuffer::Create(indices, 6, BufferUsageType::STATIC);
    _vao->setElementBuffer(_ebo);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
}

void opengl_renderer::clearColor(const glm::vec4& color){
    glClearColor(color.r, color.g, color.b, color.a);
}

void opengl_renderer::clear(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void opengl_renderer::clear_depth_buffer_bit_only(){
    glClear(GL_DEPTH_BUFFER_BIT);
}

void opengl_renderer::setViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h){
    glViewport(x, y, w, h);
}

void opengl_renderer::setLineWidth(float w){
    glLineWidth(w);
}

void opengl_renderer::drawElements(const std::shared_ptr<VertexArray>& vert_array, uint32_t idx_cnt){
    vert_array->bind();
    uint32_t cnt = idx_cnt ? idx_cnt : vert_array->getElementBuffer()->getCount();
    glDrawElements(GL_TRIANGLES, cnt, GL_UNSIGNED_INT, nullptr);
}

void opengl_renderer::drawLines(const std::shared_ptr<VertexArray>& vert_array, uint32_t vert_cnt){
    vert_array->bind();
    glDrawArrays(GL_LINES, 0, vert_cnt);
}

void opengl_renderer::bindTexture(uint32_t _id, uint32_t slot){
    glBindTextureUnit(slot, _id);
}

void opengl_renderer::toggle_cursor_input_mode(CursorInputMode mode){
    switch (mode) {

    case REV_CURSOR_DISABLED:{
        auto *window = static_cast<GLFWwindow*>(Application::getInstance().getWindow().getGLWindow());
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        break;
    }
    case REV_CURSOR_NORMAL:
        auto *window = static_cast<GLFWwindow*>(Application::getInstance().getWindow().getGLWindow());
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        break;
    }
}

void opengl_renderer::draw_full_screen_quad() {
    _vao->bind();
    glDrawElements(GL_TRIANGLES, _vao->getElementBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
}
