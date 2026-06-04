#pragma once

#include "core/layer.hpp"
#include "core/input.hpp"
#include "core/resource_manager.hpp"
#include "renderer/camera.hpp"
#include "renderer/render_system.hpp"

class SandboxLayer : public Layer{
public:
    void onAttach() override;
    // void onDetach() override;
    void onUpdate(float deltaTime) override;
private:
    ResourceManager _resource_manager;
    RenderSystem _render_system;
    Camera3D _cam;

    uint32_t _model_id;
    uint32_t active_shader_id;
};
