#include "ecs/scene.hpp"
#include "ecs/entity.hpp"
#include "ecs/components.hpp"


Scene::Scene() = default;
Scene::~Scene() {}


Entity Scene::create_entity(const std::string& name){
    Entity entity(_registry.create(), this);

    entity.addComponent<TransformComponent>();
    auto& tag = entity.addComponent<TagComponent>();
    tag._tag = name;

    return entity;
}

Entity Scene::create_spot_light(const std::string& name, glm::vec3 color){
    // Only adding the transform component and the point light component to an empty entity to create an entity that works as a point light
    Entity entity(_registry.create(), this);
    entity.addComponent<TransformComponent>();

    auto& light = entity.addComponent<SpotLightComponent>();
    light._color = color;

    auto& tag = entity.addComponent<TagComponent>();
    tag._tag = name;

    return entity;
}

Entity Scene::create_directional_light(const std::string& name, glm::vec3 color){
    Entity entity(_registry.create(), this);
    entity.addComponent<TransformComponent>();

    auto& light = entity.addComponent<DirectionalLightComponent>();
    light._color = color;

    auto& tag = entity.addComponent<TagComponent>();
    tag._tag = name;

    return entity;
}

// Entity Scene::create_camera(const std::string& name){
//     return create_entity(name);
// }

void Scene::onUpdate(float deltaTime, const Camera3D& cam, RenderSystem& render_sys, ResourceManager& res_mgr){
    render_sys.BeginFrame(cam);

    auto mesh_view = _registry.view<TransformComponent, MeshComponent>();
    for(auto entity : mesh_view){
        auto [transform, mesh_comp] = mesh_view.get<TransformComponent, MeshComponent>(entity);
        const Model& model = res_mgr.get_model(mesh_comp._model_id);
        glm::mat4 entity_transform = transform.getTransform();

        for(const auto& mesh : model._meshes){
            RenderCall packet;
            packet._shader_id = mesh_comp._shader_id;
            packet._material_id = model._material_ids[mesh._material_idx];
            packet._vao         = mesh._vert_array;
            packet._idx_count   = mesh._vert_array->getElementBuffer()->getCount();
            packet._model_matrix = entity_transform * mesh._local_transform;
            packet._entity_id = entity;
            render_sys.Submit(packet, res_mgr);
        }
    }
}

void Scene::update_active_slights(const std::vector<SpotLightComponent>& updated_data){
    auto view = _registry.view<SpotLightComponent>();
    unsigned int i = 0;
    for(auto entity : view){
        if(i >= updated_data.size()){
            break;
        }
        auto& light = view.get<SpotLightComponent>(entity);
        light = updated_data[i];
        i++;
    }
}

std::vector<SpotLightComponent> Scene::get_active_spot_lights() const {
    std::vector<SpotLightComponent> lights;
    auto view = _registry.view<SpotLightComponent>();
    for(auto entity : view) {
        lights.push_back(view.get<SpotLightComponent>(entity));
    }
    return lights;
}

DirectionalLightComponent Scene::get_directional_light() const {
    if(!_registry.storage<DirectionalLightComponent>()->empty()){
        auto entity = _registry.view<DirectionalLightComponent>().front();
        return _registry.view<DirectionalLightComponent>().get<DirectionalLightComponent>(entity);
    }
    // Return an inactive fallback if no entity contains a light component
    DirectionalLightComponent inactive_light;
    inactive_light._enabled = false;
    return inactive_light;
}

// Coast to coast, LA to Chicago, western male
// Across the north and south, to Key Largo, love for sale
// ========        =========
// ||              ||     ||
// ========        ||     ||
//       ||        ||     ||
// ======== MOOTH  ========= PERATOR
