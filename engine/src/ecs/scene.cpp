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

Entity Scene::create_point_light(const std::string& name, glm::vec3 color){
    // Only adding the transform component and the point light component to an empty entity to create an entity that works as a point light
    Entity entity(_registry.create(), this);
    entity.addComponent<TransformComponent>();

    auto& light = entity.addComponent<PointLightComponent>();
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
    render_sys.BeginFrame();

    // Extracting the point lights
    std::vector<PointLightComponent> _active_pl_data;
    auto pl_view = _registry.view<TransformComponent, PointLightComponent>();
    for(auto entity : pl_view){
        auto [transform, light_comp] = pl_view.get<TransformComponent, PointLightComponent>(entity);
        _active_pl_data.push_back(light_comp);
    }

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
            render_sys.Submit(packet, res_mgr);
        }
    }
    // This is all makeshift first draft code kind of thing.
    DirectionalLightComponent dirLight;
    if(!_registry.storage<DirectionalLightComponent>().empty()){
        auto entity = _registry.view<TransformComponent, DirectionalLightComponent>().front();
        auto [transform, light_comp] = _registry.view<TransformComponent, DirectionalLightComponent>().get<TransformComponent, DirectionalLightComponent>(entity);
        dirLight = light_comp;
        render_sys.EndFrame(res_mgr, cam.getViewProjMatrix(), cam, _active_pl_data, dirLight);
    }
    render_sys.EndFrame(res_mgr, cam.getViewProjMatrix(), cam, _active_pl_data);
}

// Coast to coast, LA to Chicago, western male
// Across the north and south, to Key Largo, love for sale
// ========        =========
// ||              ||     ||
// ========        ||     ||
//       ||        ||     ||
// ======== MOOTH  ========= PERATOR
