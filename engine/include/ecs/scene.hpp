#pragma once

#include <string>
#include <cstdint>

#include "core/resource_manager.hpp"
#include "entt/entt.hpp"
#include "renderer/camera.hpp"
#include "renderer/render_system.hpp"

class Entity;

class Scene{
public:
    Scene();
    ~Scene();

    Entity create_entity(const std::string& name = "Empty Entity");
    void onUpdate(float deltaTime, const Camera3D& cam, RenderSystem& render_sys, ResourceManager& res_mgr);

private:
    entt::registry _registry;
    friend class Entity;
};
