#include "core/sandbox_layer.hpp"
#include "events/event_dispatcher.hpp"

void SandboxLayer::onAttach(){
    _lastMouseX = 0.0F;
    _lastMouseY = 0.0F;
    _firstMouse = true;

    _resource_manager.Init();

    _cam = Camera3D(glm::vec3(0.0F, 2.0F, 4.0F));
    _cam.setProjection(glm::perspective(glm::radians(60.0F), (float)1280 / (float)720, 0.1F, 1000.0F));

    // ENSURE THE SHADER IS LOADED!
    _resource_manager.get_shader("default_shader");

    uint32_t _shader_id = _resource_manager.load_shader("cyborg_shader", "revrender/assets/models/cyborg/cyborg.vert", "revrender/assets/models/cyborg/cyborg.frag");
    _model_id = _resource_manager.load_model("revrender/assets/models/cyborg/cyborg.obj");

    Entity model_entity = _scene.create_entity("cyborg");
    // Ensure the MeshComponent is explicitly attached
    auto& model = model_entity.addComponent<MeshComponent>(MeshComponent{._model_id=_model_id});
    model._shader_id = _shader_id;

    auto& transform = model_entity.getComponent<TransformComponent>();
}


void SandboxLayer::onEvent(Event& e){
    EventDispatcher ed(e);
    ed.Dispatch<MouseMoved>([this](MouseMoved& event){
        return onMouseMoved(event);
    });
}

bool SandboxLayer::onMouseMoved(MouseMoved& e){
    auto xpos = e.X();
    auto ypos = e.Y();

    if (_firstMouse) {
        _lastMouseX = xpos;
        _lastMouseY = ypos;
        _firstMouse = false;
    }

    float xOffset = xpos - _lastMouseX;
    float yOffset = _lastMouseY - ypos;

    _lastMouseX = xpos;
    _lastMouseY = ypos;

    _cam.processMouseMovement(xOffset, yOffset);
    return false;
}

void SandboxLayer::onUpdate(float deltaTime){

    if (Input::isKeyPressed(Key::REV_KEY_W))
        _cam.processKeyboard(camera_movement::FORWARD, deltaTime);
    if (Input::isKeyPressed(Key::REV_KEY_S))
        _cam.processKeyboard(camera_movement::BACKWARD, deltaTime);
    if (Input::isKeyPressed(Key::REV_KEY_A))
        _cam.processKeyboard(camera_movement::LEFT, deltaTime);
    if (Input::isKeyPressed(Key::REV_KEY_D))
        _cam.processKeyboard(camera_movement::RIGHT, deltaTime);

    _scene.onUpdate(deltaTime, _cam, _render_system, _resource_manager);
}
