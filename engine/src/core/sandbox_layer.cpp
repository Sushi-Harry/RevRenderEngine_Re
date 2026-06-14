#include "core/sandbox_layer.hpp"
#include "events/event_dispatcher.hpp"
#include <chrono>
#include "core/application.hpp"

void SandboxLayer::onAttach(){
    _lastMouseX = 0.0F;
    _lastMouseY = 0.0F;
    _firstMouse = true;

    _resource_manager.Init();
    _render_system.Init();

    _scene_data._camera = Camera3D(glm::vec3(0.0F, 2.0F, 4.0F));
    _scene_data._camera.setProjection(glm::perspective(glm::radians(60.0F), (float)Application::getInstance().getWindow().getWidth() / (float)Application::getInstance().getWindow().getHeight(), 0.1F, 1000.0F));

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

    Entity s_light = _scene.create_spot_light();
    auto& light_comp = s_light.getComponent<SpotLightComponent>();
    light_comp._position = { 0.0,6.0, 6.0};
    light_comp._direction = {0.0, -1.0, -1.0};
    light_comp._color = {1.0, 0.0, 1.0};
    _scene_data._spot_lights.push_back(light_comp);

    Entity d_light = _scene.create_directional_light();
    auto& dirLight = d_light.getComponent<DirectionalLightComponent>();
    dirLight._direction = glm::normalize(glm::vec3(0.0f, -1.0f, -1.0f));
    dirLight._enabled = false;
    _scene_data._directional_light = dirLight;

    // Testing skybox
    _sbox = new Skybox(_resource_manager, "skybox_1", {
        "revrender/assets/skybox/basicDay/right.jpg",
        "revrender/assets/skybox/basicDay/left.jpg",
        "revrender/assets/skybox/basicDay/top.jpg",
        "revrender/assets/skybox/basicDay/bottom.jpg",
        "revrender/assets/skybox/basicDay/back.jpg",
        "revrender/assets/skybox/basicDay/front.jpg"
    });

    // Initializing the shadowmap for directional Light
    ShadowMapSpecs dirspecs = {._type=ShadowMapType::REV_SHADOW_MAP_DIRECTIONAL, ._width=1024, ._height=1024, ._slices=1 };
    _directional_shadow_map = ShadowMap::Create(dirspecs);
    // Initializing the shadowmap for spot light
    ShadowMapSpecs spotspecs = {._type=ShadowMapType::REV_SHADOW_MAP_SPOT, ._width=1024, ._height=1024, ._slices=4};
    _spot_shadow_map = ShadowMap::Create(spotspecs);
}

void SandboxLayer::onEvent(Event& e){
    EventDispatcher ed(e);
    ed.Dispatch<MouseMoved>([this](MouseMoved& event){
        return onMouseMoved(event);
    });
    ed.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& event){
        float aspect = (float)event.getWidth() / (float)event.getHeight();
        _scene_data._camera.setProjection(glm::perspective(glm::radians(60.0F), aspect, 0.1F, 1000.0F));
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

    _scene_data._camera.processMouseMovement(xOffset, yOffset);
    return false;
}

void SandboxLayer::onUpdate(float deltaTime){


    if (Input::isKeyPressed(Key::REV_KEY_W))
        _scene_data._camera.processKeyboard(camera_movement::FORWARD, deltaTime);
    if (Input::isKeyPressed(Key::REV_KEY_S))
        _scene_data._camera.processKeyboard(camera_movement::BACKWARD, deltaTime);
    if (Input::isKeyPressed(Key::REV_KEY_A))
        _scene_data._camera.processKeyboard(camera_movement::LEFT, deltaTime);
    if (Input::isKeyPressed(Key::REV_KEY_D))
        _scene_data._camera.processKeyboard(camera_movement::RIGHT, deltaTime);


    // "This may look like it's becoming cluttered,
    //          It is, in fact, becoming very cluttered." - Harry Chauhan, 11 June, 2026, 7:47 P.M.

    _scene.onUpdate(deltaTime, _scene_data._camera, _render_system, _resource_manager);
    _scene_data._directional_light = _scene.get_directional_light();
    _scene_data._spot_lights = _scene.get_active_spot_lights();

    // This is for directional light shadows
    if(_scene_data._directional_light._enabled){
        glm::mat4 lightSpaceMat = _render_system.CalculateLightSpaceMatrix(_scene_data._directional_light, _scene_data._camera);
        // Shadow map Binding
        _directional_shadow_map->BindFramebuffer();
            GeneralRenderCalls::clear_depth_buffer_bit_only();
            _scene_data._directional_light._light_space_matrix = lightSpaceMat;
            _render_system.ShadowMappingRenderPass(_resource_manager, lightSpaceMat);
        _directional_shadow_map->UnbindFramebuffer();
    }
    // This loop is for spot light shadows
    if(!_scene_data._spot_lights.empty()){
        _spot_shadow_map->BindFramebuffer();
        for(int i = 0; i < _scene_data._spot_lights.size(); i++){
            glm::mat4 lightSpaceMat = _render_system.CalculateLightSpaceMatrix(_scene_data._spot_lights[i], _scene_data._camera);
            _scene_data._spot_lights[i]._light_space_matrix = lightSpaceMat;
            _spot_shadow_map->BindLayerTarget(i);
                GeneralRenderCalls::clear_depth_buffer_bit_only();
                _render_system.ShadowMappingRenderPass(_resource_manager, lightSpaceMat);
        }
        _spot_shadow_map->UnbindFramebuffer();
    }
    auto& window = Application::getInstance().getWindow();
    GeneralRenderCalls::clear();
    _scene_data._directional_shadow_map_id = _directional_shadow_map->GetTextureID();
    _scene_data._spot_shadow_map_id = _spot_shadow_map->GetTextureID();
    _render_system.EndFrame(_resource_manager, _scene_data);
    _sbox->draw(_resource_manager, _scene_data._camera);
    _render_system.ClearRenderQueue();
}
