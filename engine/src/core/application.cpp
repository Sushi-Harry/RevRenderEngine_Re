#include "core/application.hpp"
#include "core/resource_manager.hpp"
#include "renderer/camera.hpp"
#include "renderer/render_system.hpp"
#include <iostream>

// Setting the _instance to null pointer first
Application* Application::_instance =nullptr;

Application::Application(Window* window, const std::string& name) : _window(window), _gui(new gui_layer()){
    // set _instance pointer to this pointer. Why? Well causes THIS is the current instance (~_~)
    _instance = this;

    // Setting the event callback
    _window->setEventCallback([this](Event& ev){ this->onEvent(ev); });
    _layer_stack.push_overlay(_gui);
}

// Empty deconstructor
Application::~Application() {}

// Event Dispatcher / Event Handling code
void Application::onEvent(Event& e){
    EventDispatcher ed(e);

    ed.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& event){ return onWindowResize(event); });
    ed.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& event){ return onWindowClose(event); });

    for(auto iter = _layer_stack.rbegin(); iter != _layer_stack.rend(); iter++){
        if(e.handled){
            break;
        }
        (*iter)->onEvent(e);
    }
}

bool Application::onWindowClose(WindowCloseEvent& e){
    _isRunning = false;
    return true;
}

bool Application::onWindowResize(WindowResizeEvent& e){
    return e.getWidth() != 0 && e.getHeight() != 0;
}

void Application::Close(){
    _gui->onDetach();
    _isRunning = false;
}

void Application::Run(){
    ResourceManager resource_manager;
    resource_manager.Init();
    RenderSystem render_sys;

    Camera3D cam(glm::vec3(0.0F, 0.0F, 5.0F));
    cam.setProjection(glm::perspective(glm::radians(45.0F), (float)getInstance().getWindow().getWidth() / (float)getInstance().getWindow().getHeight() , 0.1F, 1000.0F));
    uint32_t model_id = resource_manager.load_model("revrender/assets/core/default_rev/default_rev.fbx");
    const Model& model = resource_manager.get_model(model_id);

    std::shared_ptr<Shader> def_shader = resource_manager.get_shader("default_shader");
    uint32_t active_shader_id = 0;

    while (_isRunning) {
        // this is the main rendering loop
        // Clear the buffer here
        // poll events here.
        // draw stuff here
        // Do everything here

        GeneralRenderCalls::clearColor({0.0f, 0.0f, 0.0f, 0.0f});
        GeneralRenderCalls::clear();

        render_sys.BeginFrame();
        auto model_transform = glm::mat4(1.0F);
        model_transform = glm::scale(model_transform, glm::vec3(0.1F));
        model_transform = glm::rotate(model_transform, glm::radians(-90.0F), glm::vec3(0.0, 1.0, 0.0));
        for (const auto& mesh : model._meshes) {
            RenderCall packet;
            packet._shader_id   = active_shader_id;
            packet._material_id = mesh._material_id;
            packet._vao         = mesh._vert_array;
            packet._idx_count   = mesh._vert_array->getElementBuffer()->getCount();
            packet._model_matrix = model_transform;

            render_sys.Submit(packet);
        }
        render_sys.EndFrame(resource_manager, cam.getViewProjMatrix());

        _gui->begin();
        _gui->end();

        // This will poll events and swap buffers
        _window->onUpdate();
    }
}
