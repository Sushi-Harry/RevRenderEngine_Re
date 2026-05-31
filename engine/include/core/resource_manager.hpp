#pragma once

#include "renderer/shader.hpp"
#include "ecs/components.hpp"
#include "renderer/texture.hpp"

#include <unordered_map>

class ResourceManager{
public:
    uint32_t load_texture(const std::string& path, REV_TEXTURE_TYPE type);
    std::shared_ptr<Texture2D> get_texture(const std::string& path) const;

    uint32_t load_material(const Material& mat);
    Material& get_material() const;

    uint32_t load_shader(const std::shared_ptr<Shader>& shader);
    std::shared_ptr<Shader> get_shader(uint32_t id) const;

    uint32_t load_model(const std::shared_ptr<Model>& model);
    std::shared_ptr<Model> get_model(const std::string& path) const;

private:
    uint32_t _next_tex_id = 1;
    uint32_t _next_mat_id = 1;
    uint32_t _next_shader_id = 1;
    uint32_t _next_model_id = 1;

    std::unordered_map<uint32_t, std::shared_ptr<Model>> _models;
    std::unordered_map<uint32_t, std::shared_ptr<Shader>> _shaders;
    std::unordered_map<uint32_t, std::shared_ptr<Texture2D>> _textures_2d;
    std::unordered_map<uint32_t, Material> _materials;
};
