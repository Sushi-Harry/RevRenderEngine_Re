#include "glad/glad.h"


#include "core/sandbox_layer.hpp"
#include "ImGuizmo.h"

void SandboxLayer::colorPicking(Framebuffer& _imgui_fbo){
    if(ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGuizmo::IsOver()){
        ImVec2 mouse_pos = ImGui::GetMousePos();
        ImVec2 image_min = ImGui::GetItemRectMin();
        ImVec2 image_max = ImGui::GetItemRectMax();

        int mouseX = static_cast<int>(mouse_pos.x - image_min.x);
        int mouseY = static_cast<int>(mouse_pos.y - image_min.y);

        int imageHeight = static_cast<int>(image_max.y - image_min.y);
        int opengl_y = (imageHeight - 1) - mouseY;

        // std::cout << "[DEBUG]::Mouse X: " << mouseX << " | OpenGL Y: " << opengl_y << '\n';

        if(mouseX >= 0 && opengl_y >= 0 && mouseX < (int)_viewport_size.x && opengl_y < (int)_viewport_size.y){
            _imgui_fbo.bind();
                glReadBuffer(GL_COLOR_ATTACHMENT1);
                int pixelData = -1;
                glReadPixels(mouseX, opengl_y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
            _imgui_fbo.unbind();

            if(pixelData != -1){
                _selected_entity_id = static_cast<entt::entity>(pixelData);
                // std::cout << "[LOG]::COLOR_PICKING::SELECTED_ENTITY_ID::" << pixelData << '\n';
            }else{
                _selected_entity_id = entt::null;
            }
        }
    }
}
