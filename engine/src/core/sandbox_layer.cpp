#include "core/sandbox_layer.hpp"
#include "core/delta_time.hpp"

void SandboxLayer::onAttach(){
    _resource_manager.Init();
    _cam = Camera3D(glm::vec3(0.0F));
    _cam.setProjection(glm::perspective(glm::radians(60.0F), (float)1280 / (float)720, 0.1F, 1000.0F));
    _model_id = _resource_manager.load_model("revrender/assets/core/default_rev/default_rev.fbx");
    active_shader_id = 0;
}

void SandboxLayer::onUpdate(float deltaTime){

    if (Input::isKeyPressed(Key::REV_KEY_W)) {
        _cam.processKeyboard(camera_movement::FORWARD, deltaTime);
        std::cout << "PRESSED W\n";
    }
    if (Input::isKeyPressed(Key::REV_KEY_S))
        _cam.processKeyboard(camera_movement::BACKWARD, deltaTime);
    if (Input::isKeyPressed(Key::REV_KEY_A))
        _cam.processKeyboard(camera_movement::LEFT, deltaTime);
    if (Input::isKeyPressed(Key::REV_KEY_D))
        _cam.processKeyboard(camera_movement::RIGHT, deltaTime);


    _render_system.BeginFrame();
    auto model_transform = glm::mat4(1.0F);
    model_transform = glm::translate(model_transform, glm::vec3(-10.0f, -20.0f, -100.0f));
    model_transform = glm::scale(model_transform, glm::vec3(0.01F));
    model_transform = glm::rotate(model_transform, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    const Model& model = _resource_manager.get_model(_model_id);
    for (const auto& mesh : model._meshes) {
        RenderCall packet;
        packet._shader_id   = active_shader_id;
        packet._material_id = mesh._material_id;
        packet._vao         = mesh._vert_array;
        packet._idx_count   = mesh._vert_array->getElementBuffer()->getCount();
        packet._model_matrix = model_transform * mesh._local_transform;
        _render_system.Submit(packet);
    }
    _render_system.EndFrame(_resource_manager, _cam.getViewProjMatrix());
}
