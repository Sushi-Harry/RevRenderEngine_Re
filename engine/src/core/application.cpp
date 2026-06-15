#include "core/application.hpp"
#include "core/resource_manager.hpp"
#include "core/sandbox_layer.hpp"
#include "renderer/camera.hpp"
#include "renderer/render_system.hpp"
#include <iostream>

// Setting the _instance to null pointer first
Application* Application::_instance =nullptr;

Application::Application(Window* window, const std::string& name) : _window(window), _gui(new gui_layer()){
    // set _instance pointer to this pointer. Why? Well causes THIS is the current instance (~_~)
    _instance = this;
    _layer_stack.push_overlay(_gui);
    _layer_stack.push_layer(new SandboxLayer());
    _window->setEventCallback([this](Event& ev){ this->onEvent(ev); });
    GeneralRenderCalls::Init();
    Input::Init();
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
    if(e.getWidth() == 0 || e.getHeight() == 0) {
        return false;
    }
    GeneralRenderCalls::setViewport(0, 0, e.getWidth(), e.getHeight());
    return false;
}

void Application::Close(){
    _gui->onDetach();
    _isRunning = false;
}

void Application::Run(){

    while (_isRunning) {
        // this is the main rendering loop
        // Clear the buffer here
        // poll events here.
        // draw stuff here
        // Do everything here
        Time::calculateDeltaTime();
        GeneralRenderCalls::clearColor({0.0F, 0.0F, 0.0F, 0.0F});
        GeneralRenderCalls::clear();

        // Layer Updates
        for(Layer* layer : _layer_stack){
            layer->onUpdate(Time::deltaTime());
        }

        // GUI RENDERING
        _gui->begin();
        for(Layer* layer : _layer_stack){
            layer->onRenderGUI();
        }
        _gui->end();

        // This will poll events and swap buffers
        _window->onUpdate();
    }
}
