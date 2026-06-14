#pragma once

#include "ecs/components.hpp"
#include "renderer/camera.hpp"

struct SceneData{
    Camera3D _camera;
    DirectionalLightComponent _directional_light;
    std::vector<SpotLightComponent> _spot_lights;

    uint32_t _directional_shadow_map_id = 0;
    uint32_t _spot_shadow_map_id = 0;
};
