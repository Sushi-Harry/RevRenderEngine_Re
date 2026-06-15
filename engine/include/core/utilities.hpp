#pragma once
#include <cstdint>

enum TextureSlots : uint32_t {
    // This is not global. This will be used per-model
    REV_MATERIAL_DIFFUSE = 0,
    REV_MATERIAL_SPECULAR = 1,
    REV_MATERIAL_NORMAL = 2,

    // This is global data
    REV_SHADOWMAP_DIRECTIONAL = 3,
    REV_SHADOWMAP_SPOTLIGHT = 4,
    REV_ENVIRONMENT_SKYBOX = 5
};

enum CursorInputMode : uint8_t{
    REV_CURSOR_DISABLED,
    REV_CURSOR_NORMAL
};
