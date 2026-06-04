#pragma once

#include <string>
#include "events/events.hpp"

class Layer{
public:
    Layer(const std::string& name = "layer") : _name(name) {}
    virtual ~Layer() = default;

    virtual void onAttach() {}
    virtual void onDetach() {}
    virtual void onUpdate(float deltaTime) {}
    virtual void onRenderGUI() {}
    virtual void onEvent(Event& e) {}

    const char* getName() const { return _name.c_str(); }
private:
    std::string _name;
};
