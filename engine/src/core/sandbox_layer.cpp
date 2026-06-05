#include "core/sandbox_layer.hpp"
#include "core/delta_time.hpp"

void SandboxLayer::onAttach(){
    _resource_manager.Init();

    _cam = Camera3D(glm::vec3(0.0F, 2.0F, 4.0F));
    _cam.setProjection(glm::perspective(glm::radians(60.0F), (float)1280 / (float)720, 0.1F, 1000.0F));

    // ENSURE THE SHADER IS LOADED!
    _resource_manager.get_shader("default_shader");

    _model_id = _resource_manager.load_model("revrender/assets/models/cyborg/cyborg.obj");

    Entity model_entity = _scene.create_entity("cyborg");

    // Ensure the MeshComponent is explicitly attached
    model_entity.addComponent<MeshComponent>(MeshComponent{_model_id});

    // Apply the local math fixes
    auto& transform = model_entity.getComponent<TransformComponent>();
    // transform._scale = glm::vec3(0.1f);
    // transform._rotation = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);
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


    _scene.onUpdate(deltaTime, _cam, _render_system, _resource_manager);
}
