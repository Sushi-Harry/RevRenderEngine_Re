#include "renderer/render_calls.hpp"
#include "glad/glad.h"
void DrawCommands::DrawIndexed(const std::shared_ptr<VertexArray> &vao, uint32_t indexCount){
    vao->bind();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}

void DrawCommands::DrawIndexed(uint32_t indexCount){
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}

void DrawCommands::UnbindVAO(){
    glBindVertexArray(0);
}
