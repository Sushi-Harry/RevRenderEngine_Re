#include "opengl_renderer.hpp"

std::unique_ptr<RenderingAPI> RenderingAPI::create(){
    return std::make_unique<opengl_renderer>();
}

void opengl_renderer::Init(){
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
