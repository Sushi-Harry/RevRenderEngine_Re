#pragma once
#include <cstdint>
#include <functional>
#include <string>
#include <utility>
#include "events/events.hpp"

struct WindowProperties{
    std::string name;
    uint32_t height;
    uint32_t width;

    // Constructor for the properties
    WindowProperties(std::string  title = "Rev Render Engine", uint32_t W = 1280, uint32_t H = 720) : name(std::move(title)), width(W), height(H) {}
};

class Window{
public:
    using EventCallbackFn = std::function<void(Event&)>;

    virtual ~Window() = default;
    virtual void onUpdate() = 0;
    virtual uint32_t getWidth() = 0;
    virtual uint32_t getHeight() = 0;

    // Window attributes
    virtual void setEventCallback(const EventCallbackFn& callback)= 0;
    virtual void* getGLWindow() const = 0;

    static Window* create(const WindowProperties& prop = WindowProperties());
};
