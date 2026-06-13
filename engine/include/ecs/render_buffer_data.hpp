#pragma once

#include "glm/glm.hpp"

// These structs match the layouts in the default shader files

// 48+64 Bytes after 8 bytes of padding
struct DirectionalLightBufferData{
    // 16 Bytes
    glm::vec3 _direction;
    uint32_t _enabled;

    // 16 Bytes
    glm::vec3 _color;
    float _ambient;

    // 64 Bytes
    glm::mat4 _light_space_matrix;

    // 8 Bytes
    float _specular;
    float _diffuse;
    // These padding variables are just here to take extra space in order to match the size of the structs in the glsl code
    // Another 8 bytes padded
    float Padding[2];
};

// 64 Bytes (after 12 bytes of padding)
struct PointLightBufferData{
    // 16 Bytes
    glm::vec3 _position;
    uint32_t _enabled;

    // 16 Bytes
    glm::vec3 _color;
    float _ambient;

    // 16 Bytes
    float _constant;
    float _linear;
    float _quadratic;
    float _specular;

    // 4 Bytes
    float _diffuse;

    // 12 bytes of padding
    float Padding[3];
};

// 144 Bytes (after 8 bytes of padding)
struct SpotLightBufferData{
    // 16 Bytes
    glm::vec3 _position;
    uint32_t _enabled;

    // 16 Bytes
    glm::vec3 _direction;
    float _constant;

    // 16 Bytes
    glm::vec3 _color;
    float _linear;

    // 64 Bytes
    glm::mat4 _light_space_matrix;

    // 16 Bytes
    float _quadratic;
    float _ambient;
    float _specular;
    float _diffuse;

    // 8 Bytes.
    float _inner_cutoff;
    float _outer_cutoff;

    // Another padded 8 Bytes.
    float Padding[2];
};


// Binding = 0
// 208 Bytes
struct CameraBufferData {
    // 64 Bytes
    glm::mat4 View;

    // 64 Bytes
    glm::mat4 Projection;

    // 64 Bytes
    glm::mat4 ViewProjection;

    // 16 Bytes
    glm::vec3 ViewPosition;

    // 4 Bytes of padding
    float Padding;
};

// Binding = 1
struct LightBufferData{
    DirectionalLightBufferData _directional_light;
    PointLightBufferData _point_lights[4];
    SpotLightBufferData _spot_lights[4];
};
