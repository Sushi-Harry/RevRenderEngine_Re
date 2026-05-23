#include "core/application.hpp"
#include <iostream>

// Setting the _instance to null pointer first
Application* Application::_instance =nullptr;

Application::Application(Window* window, const std::string& name) : _window(window){
    // set _instance pointer to this pointer. Why? Well causes THIS is the current instance (~_~)
    _instance = this;

    // Setting the event callback
    _window->setEventCallback([this](){ this->Close(); });
}

// Empty deconstructor
Application::~Application() {}

void Application::Close(){
    _isRunning = false;
}

void Application::Run(){
    while (_isRunning) {
        // this is the main rendering loop
        // Clear the buffer here
        // poll events here.
        // draw stuff here
        // Do everything here

        // This will poll events and swap buffers
        _window->onUpdate();
    }
}
