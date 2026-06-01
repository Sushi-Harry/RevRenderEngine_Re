#pragma once
#include <cstdint>
#include <vector>
#include "renderer/vertex_array.hpp"
#include "glm/glm.hpp"

enum class REV_TEXTURE_TYPE{
    REV_DIFFUSE,
    REV_SPECULAR,
    REV_NORMAL,
    REV_NONE
};

struct Material{
    std::vector<uint32_t> _diffuse_textures;
    std::vector<uint32_t> _specular_textures;
    std::vector<uint32_t> _normal_textures;
    uint32_t _id;
};

struct Mesh{
    std::shared_ptr<VertexArray> _vert_array;
    uint32_t _material_id;
};

struct Model{
    std::vector<Mesh> _meshes;
    std::string _path;
    uint32_t _id;
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

struct TransformComponent{
    glm::vec3 _position = {0, 0, 0};
    glm::vec3 _rotation = {0, 0, 0};
    glm::vec3 _scale    = {1.0F, 1.0F, 1.0F};

    glm::mat4 _local_matrix = glm::mat4(1.0F);
    glm::mat4 _model_matrix = glm::mat4(1.0F);

    uint32_t _parent = 0; // 0 for root object / for an object with no parents (orphan?)
    std::vector<uint32_t> _children;
};
