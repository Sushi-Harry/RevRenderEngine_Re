#pragma once

#include "core/layer.hpp"
#include "core/input.hpp"
#include "core/resource_manager.hpp"
#include "events/key_events.hpp"
#include "events/mouse_events.hpp"
#include "imgui.h"
#include "renderer/camera.hpp"
#include "renderer/framebuffer.hpp"
#include "renderer/render_system.hpp"

#include "ecs/scene.hpp"
#include "ecs/entity.hpp"
#include "renderer/renderables.hpp"
#include "renderer/shadow_map.hpp"
#include "renderer/skybox.hpp"

class SandboxLayer : public Layer{
public:
    void onAttach() override;
    // void onDetach() override;
    void onEvent(Event& e) override;
    void onRenderGUI() override;
    void onUpdate(float deltaTime) override;
private:
    bool onMouseMoved(MouseMoved& e);
    bool onKeyReleased(KeyReleased& e);
    void drawSceneHierarchyPanel();
    float _lastMouseX, _lastMouseY;
    bool _firstMouse;

    ResourceManager _resource_manager;
    RenderSystem _render_system;
    Skybox* _sbox;

    Scene _scene;
    SceneData _scene_data;

    std::shared_ptr<Framebuffer> _imgui_fbo;

    std::shared_ptr<ShadowMap> _directional_shadow_map;
    std::shared_ptr<ShadowMap> _spot_shadow_map;

    uint32_t _model_id;
    uint32_t active_shader_id;

    ImVec2 _viewport_size = {1440.0F, 810.0F};

    entt::entity _selected_entity_id;

    bool _viewport_focused = true;
};
