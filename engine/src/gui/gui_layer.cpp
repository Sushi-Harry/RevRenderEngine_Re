#include "gui/gui_layer.hpp"
#include "core/application.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

void gui_layer::onAttach() {
    // Imgui setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    Application& app = Application::getInstance();
    auto *window = static_cast<GLFWwindow*>(app.getWindow().getGLWindow());

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void gui_layer::onDetach(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void gui_layer::begin(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
}

void gui_layer::end(){
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void gui_layer::onEvent(Event& e){
    if(_blockEvents){
        ImGuiIO& io = ImGui::GetIO();
        e.handled |= e.isInCategory(EventCategory::EVENT_CATEGORY_KEYBOARD) & io.WantCaptureKeyboard;
        e.handled |= e.isInCategory(EventCategory::EVENT_CATEGORY_MOUSE) & io.WantCaptureMouse;
    }
}
