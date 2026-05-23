#pragma once
#include "core/window.hpp"
#include <memory>

class Application{
public:
    Application(Window* window, const std::string& app_name = "RevRenderRE");
    virtual ~Application();

    // This is the main rendering loop
    void Run();
    // This will be called when the application has to be closed
    void Close();
    // Returns the address of window instance
    Window& getWindow() { return *_window; }
    // Returns current application instance's pointer
    static Application* getInstance() { return _instance; }

private:
    // This is a callback for when window is closed
    bool onWindowClose();

    std::unique_ptr<Window> _window;
    bool _isRunning = true;
    static Application* _instance;
};

Application* CreateApplication();
