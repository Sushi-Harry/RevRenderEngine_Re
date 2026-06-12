#include "core/sandbox_layer.hpp"
#include "events/event_dispatcher.hpp"
#include <chrono>
#include "core/application.hpp"

void SandboxLayer::onAttach(){
    _lastMouseX = 0.0F;
    _lastMouseY = 0.0F;
    _firstMouse = true;

    _resource_manager.Init();

    _cam = Camera3D(glm::vec3(0.0F, 2.0F, 4.0F));
    _cam.setProjection(glm::perspective(glm::radians(60.0F), (float)Application::getInstance().getWindow().getWidth() / (float)Application::getInstance().getWindow().getHeight(), 0.1F, 1000.0F));

    // ENSURE THE SHADER IS LOADED!
    _resource_manager.get_shader("default_shader");

    // uint32_t _shader_id = _resource_manager.load_shader("cyborg_shader", "revrender/assets/models/cyborg/cyborg.vert", "revrender/assets/models/cyborg/cyborg.frag");
    uint32_t _shader_id = _resource_manager.load_shader("cyborg_shader", "revrender/assets/core/default_lit_shader.vert", "revrender/assets/core/default_lit_shader.frag");
    _model_id = _resource_manager.load_model("revrender/assets/models/cyborg/cyborg.obj");

    Entity model_entity = _scene.create_entity("cyborg");
    // Ensure the MeshComponent is explicitly attached
    auto& model = model_entity.addComponent<MeshComponent>(MeshComponent{._model_id=_model_id});
    model._shader_id = _shader_id;

    uint32_t _flat_plane_id = _resource_manager.load_model("revrender/assets/models/ground_flat/ground.obj");
    Entity flat_plane = _scene.create_entity("flat_plane");
    uint32_t _flat_shader_id = _resource_manager.load_shader("flat_plane_shader", "revrender/assets/core/default_lit_shader.vert", "revrender/assets/core/default_lit_shader.frag");
    auto& flat_plane_model = flat_plane.addComponent<MeshComponent>(MeshComponent{._model_id=_flat_plane_id});
    flat_plane_model._shader_id = _flat_shader_id;
    auto& transform = flat_plane.getComponent<TransformComponent>();
    transform.setPosition(glm::vec3(0.0F, -5.0F, -2.0F));
    transform.setScale(glm::vec3(8.0F));
    transform.setRotation(glm::vec3(0.0F, 0.0F, 0.0F));

    // Entity p_light = _scene.create_point_light();
    // auto& light_comp = p_light.getComponent<PointLightComponent>();
    // light_comp._position = { 0.0, 0.0, 3.0};

    Entity d_light = _scene.create_directional_light();
    auto& dl_comp = d_light.getComponent<DirectionalLightComponent>();
    dl_comp._direction = glm::normalize(glm::vec3(0.0f, -1.0f, -1.0f));

    // Testing skybox
    _sbox = new Skybox(_resource_manager, "skybox_1", {
        "revrender/assets/skybox/basicDay/right.jpg",
        "revrender/assets/skybox/basicDay/left.jpg",
        "revrender/assets/skybox/basicDay/top.jpg",
        "revrender/assets/skybox/basicDay/bottom.jpg",
        "revrender/assets/skybox/basicDay/back.jpg",
        "revrender/assets/skybox/basicDay/front.jpg"
    });

    // Initializing the framebuffer for shadow mapping
    FramebufferSpecs _shadow_specs({FramebufferTextureFormat::REV_FB_DEPTH32F}, 1024, 1024);
    _shadow_fbo = Framebuffer::Create(_shadow_specs);

}

void SandboxLayer::onEvent(Event& e){
    EventDispatcher ed(e);
    ed.Dispatch<MouseMoved>([this](MouseMoved& event){
        return onMouseMoved(event);
    });
    ed.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& event){
        float aspect = (float)event.getWidth() / (float)event.getHeight();
        _cam.setProjection(glm::perspective(glm::radians(60.0F), aspect, 0.1F, 1000.0F));
        return false;
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


    // "This may look like it's becoming cluttered,
    //          It is, in fact, becoming very cluttered." - Harry Chauhan, 11 June, 2026, 7:47 P.M.

    _scene.onUpdate(deltaTime, _cam, _render_system, _resource_manager);
    auto dirLight = _scene.get_directional_light();
    auto pointLights = _scene.get_active_point_lights();
    if(dirLight._enabled){
        glm::mat4 lightSpaceMat = _render_system.CalculateLightSpaceMatrix(dirLight, _cam);
        _shadow_fbo->bind();
        GeneralRenderCalls::clear_depth_buffer_bit_only();
        _render_system.ShadowMappingRenderPass(_resource_manager, lightSpaceMat);
        _shadow_fbo->unbind();
    }
    auto& window = Application::getInstance().getWindow();
    GeneralRenderCalls::clear();
    uint32_t depth_map_texture = _shadow_fbo->get_depth_attachment_id();
    _render_system.EndFrame(_resource_manager, _cam.getViewProjMatrix(), _cam, pointLights, dirLight, depth_map_texture);
    _sbox->draw(_resource_manager, _cam);
    _render_system.ClearRenderQueue();
}
