#include "core/utilities.hpp"
#include "renderer/render_calls.hpp"
#include "renderer/skybox.hpp"
#include "core/resource_manager.hpp"
#include "renderer/shader.hpp"
#include "renderer/camera.hpp"

#include "glad/glad.h"

Skybox::Skybox(ResourceManager& res_mgr, const std::string& name, const std::vector<std::string>& faces_path) : _cubemap_id(res_mgr.load_cubemap(name, faces_path)), _name(name){
    draw_setup();
}

void Skybox::draw_setup(){
    // The 8 unique coordinates forming a unit box centered at origin
    float vertices[] = {
        -1.0f,  1.0f, -1.0f, // 0
        -1.0f, -1.0f, -1.0f, // 1
         1.0f, -1.0f, -1.0f, // 2
         1.0f,  1.0f, -1.0f, // 3
        -1.0f,  1.0f,  1.0f, // 4
        -1.0f, -1.0f,  1.0f, // 5
         1.0f, -1.0f,  1.0f, // 6
         1.0f,  1.0f,  1.0f  // 7
    };

    // Index mappings to safely project the triangles inward
    uint32_t indices[] = {
        1, 0, 3, 3, 2, 1, // Back
        5, 4, 0, 0, 1, 5, // Left
        2, 3, 7, 7, 6, 2, // Right
        5, 6, 7, 7, 4, 5, // Front
        4, 7, 3, 3, 0, 4, // Top
        1, 2, 6, 6, 5, 1  // Bottom
    };

    auto vbo = VertexBuffer::Create(vertices, sizeof(vertices), BufferUsageType::STATIC);
    _vao = VertexArray::Create();
    BufferLayout layout = {
        { "aPosition", ShaderDataType::FLOAT3 }
    };
    vbo->setLayout(layout);
    _vao->addVertexBuffer(vbo);

    auto ebo = ElementBuffer::Create(indices, 36, BufferUsageType::STATIC);
    _vao->setElementBuffer(ebo);
}

void Skybox::draw(ResourceManager& res_mgr, const Camera3D& cam, const std::string& cmap_name){
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    auto shader = res_mgr.get_shader("skybox");
    auto cmap = res_mgr.get_cubemap(_cubemap_id);
    shader->bindShader();
    glm::mat4 view = glm::mat4(glm::mat3(cam.getViewMatrix()));
    glm::mat4 view_proj = cam.getProjectionMatrix() * view;
    shader->setMat4("u_ViewProjection", view_proj);
    cmap->bind(TextureSlots::REV_ENVIRONMENT_SKYBOX);
    shader->setInt("u_Skybox", TextureSlots::REV_ENVIRONMENT_SKYBOX);
    DrawCommands::DrawIndexed(_vao, 36);

    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
}
