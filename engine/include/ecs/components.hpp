#pragma once
#include <cstdint>
#include <utility>
#include <vector>
#include "renderer/shader.hpp"
#include "renderer/vertex_array.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum class REV_TEXTURE_TYPE : uint8_t{
    REV_DIFFUSE,
    REV_SPECULAR,
    REV_NORMAL,
    REV_NONE
};

struct Mesh{
    std::shared_ptr<VertexArray> _vert_array;
    glm::mat4 _local_transform;
    uint32_t _material_id;
};

struct Model{
    std::vector<Mesh> _meshes;
    std::string _path;
};

struct TextureComponent{
    uint32_t _tex_id;
    std::string _path;
    REV_TEXTURE_TYPE _tex_type;
};

struct VertexComponent{
    glm::vec3 _position;
    glm::vec3 _normal;
    glm::vec2 _tex_coords;
};

// USED IN THE ECS IMPLEMENTATION
struct TransformComponent{
    glm::vec3 _position = {0, 0, 0};
    glm::vec3 _rotation = {0, 0, 0};
    glm::vec3 _scale    = {1.0F, 1.0F, 1.0F};
    glm::mat4 _model_matrix = glm::mat4(1.0F);

    glm::mat4 getTransform() const {
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), _rotation.x, { 1, 0, 0 }) * glm::rotate(glm::mat4(1.0f), _rotation.y, { 0, 1, 0 }) * glm::rotate(glm::mat4(1.0f), _rotation.z, { 0, 0, 1 });
        return glm::translate(glm::mat4(1.0f), _position) * rotation * glm::scale(glm::mat4(1.0f), _scale);
    }
};

struct TagComponent{
    std::string _tag;
};

struct MeshComponent{
    uint32_t _model_id;
};

struct PointLightComponent{
    bool _enabled = true;

    glm::vec3 _position = {0.0, 0.0, 0.0};
    glm::vec3 _color = { 1.0, 1.0, 1.0 };

    // Attenuation variables
    float _constant = 1.0F;
    float _linear = 0.0F;
    float _quadratic = 0.0F;

    float _diffuse = 1.0F;
    float _ambient = 0.02F;
    float _specular = 1.0F;
};

struct CameraComponent{
    uint32_t _id;
};
