#pragma once

#include "core/layer.hpp"
#include "core/input.hpp"
#include "core/resource_manager.hpp"
#include "events/mouse_events.hpp"
#include "renderer/camera.hpp"
#include "renderer/framebuffer.hpp"
#include "renderer/render_system.hpp"

#include "ecs/scene.hpp"
#include "ecs/entity.hpp"
#include "renderer/skybox.hpp"

class SandboxLayer : public Layer{
public:
    void onAttach() override;
    // void onDetach() override;
    void onEvent(Event& e) override;
    void onUpdate(float deltaTime) override;
private:
    bool onMouseMoved(MouseMoved& e);
    float _lastMouseX, _lastMouseY;
    bool _firstMouse;

    ResourceManager _resource_manager;
    RenderSystem _render_system;
    Camera3D _cam;
    Skybox* _sbox;

    Scene _scene;

    std::shared_ptr<Framebuffer> _shadow_fbo;

    uint32_t _model_id;
    uint32_t active_shader_id;
};
