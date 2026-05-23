#pragma once
#include <GLFW/glfw3.h>
#include "core/window.hpp"

class glfw_window : public Window{
public:
    glfw_window(const WindowProperties& winProp);
    virtual ~glfw_window();
    void onUpdate() override;

    uint32_t getWidth() override { return _data.width; }
    uint32_t getHeight() override { return _data.height; }

    void setEventCallback(const EventCallbackFn& callback) override { _data.EventCallback = callback; }
    void* getWindow() override { return  _window; }

private:
    virtual void Init(const WindowProperties& props);
    virtual void Shutdown();


    GLFWwindow* _window;

    struct WinData{
        std::string title;
        uint32_t width, height;
        EventCallbackFn EventCallback;
    };

    WinData _data;
};
