#pragma once

#include <string>
#include <cstdint>

#include "core/resource_manager.hpp"
#include "entt/entt.hpp"
#include "renderer/camera.hpp"
#include "renderer/render_system.hpp"
#include "renderer/skybox.hpp"

class Entity;

class Scene{
public:
    Scene();
    ~Scene();

    // Creates a kind of default entity that has just the transform component and the tag component.
    Entity create_entity(const std::string& name = "Empty Entity");
    // Creates a point light type of entity
    Entity create_spot_light(const std::string& name = "Point Light Object", glm::vec3 color = {1.0, 1.0, 1.0});
    // Creates a DIrectional Light type of entity
    Entity create_directional_light(const std::string& name = "Directional Light Object", glm::vec3 color = {1.0, 1.0, 1.0});
    //  ============================================
    //  || THIS PART IS STILL A WORK IN PROGRESS! ||
    //  ============================================
    // Entity create_camera(const std::string& name = "Camera Object");

    const entt::registry& get_registry() const { return _registry; }
    entt::registry& get_registry() { return _registry; }

    void onUpdate(float deltaTime, const Camera3D& cam, RenderSystem& render_sys, ResourceManager& res_mgr);
    void update_active_slights(const std::vector<SpotLightComponent>& updated_data);

    std::vector<SpotLightComponent> get_active_spot_lights() const;
    DirectionalLightComponent get_directional_light() const;

private:
    entt::registry _registry;

    // Skybox Variables
    uint32_t _skybox_id = 0;

    friend class Entity;
};
