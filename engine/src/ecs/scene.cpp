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
    Entity entity(_registry.create(), this);
    entity.addComponent<TransformComponent>();

    auto& light = entity.addComponent<PointLightComponent>();
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

    auto view = _registry.view<TransformComponent, MeshComponent>();

    for(auto entity : view){
        auto [transform, mesh_comp] = view.get<TransformComponent, MeshComponent>(entity);
        const Model& model = res_mgr.get_model(mesh_comp._model_id);
        glm::mat4 entity_transform = transform.getTransform();

        for(const auto& mesh : model._meshes){
            RenderCall packet;
            packet._shader_id   = res_mgr.load_shader("cyborg_model", "revrender/assets/models/cyborg/cyborg.vert", "revrender/assets/models/cyborg/cyborg.frag");
            packet._material_id = mesh._material_id;
            packet._vao         = mesh._vert_array;
            packet._idx_count   = mesh._vert_array->getElementBuffer()->getCount();
            packet._model_matrix = entity_transform * mesh._local_transform;
            render_sys.Submit(packet, res_mgr);
        }
    }
    render_sys.EndFrame(res_mgr, cam.getViewProjMatrix());
}
