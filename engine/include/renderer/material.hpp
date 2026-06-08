#pragma once

#include <cstdint>
#include <utility>
#include <vector>
#include <memory>
#include "renderer/shader.hpp"

class ResourceManager;

class Material{
public:
    uint32_t _diffuse_texture = 0;
    uint32_t _specular_texture = 0;
    uint32_t _normal_textures = 0;
    float _shininess = 32.0F;
    uint32_t _id;
    Material() = default;

    // This is the meat and potatoes of this class. This is the reason I went through the trouble of modifying this from a basic struct to a separate class
    void Apply(ResourceManager& res_mgr, std::shared_ptr<Shader> _shader) const;
    std::shared_ptr<Shader> _shader;
};
