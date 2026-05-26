#include "core/application.hpp"
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
    while (_isRunning) {
        // this is the main rendering loop
        // Clear the buffer here
        // poll events here.
        // draw stuff here
        // Do everything here
        _gui->begin();
        _gui->end();

        // This will poll events and swap buffers
        _window->onUpdate();
    }
}
