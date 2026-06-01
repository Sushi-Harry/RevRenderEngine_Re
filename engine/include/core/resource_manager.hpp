#pragma once

#include "renderer/shader.hpp"
#include "ecs/components.hpp"
#include "renderer/texture.hpp"

#include <unordered_map>

class ResourceManager{
public:
    void Init();

    // Texture Loading and Management
    uint32_t load_texture(const std::string& path, REV_TEXTURE_TYPE type);
    uint32_t load_texture(const std::shared_ptr<Texture2D>& tex);
    std::shared_ptr<Texture2D> get_texture(uint32_t) const;
    std::shared_ptr<Texture2D> get_texture(const std::string& path) const;


    // Material Loading and Management
    uint32_t load_material(const std::string& name, const Material& mat);
    uint32_t load_material(const std::string& name, const std::vector<uint32_t>& diffuse, const std::vector<uint32_t>& specular);
    const Material& get_material(uint32_t) const;

    // Shader Loading and Management
    uint32_t load_shader(const std::string& name, const std::shared_ptr<Shader>& shader);
    uint32_t load_shader(const std::string& name, const std::string& v_path, const std::string& f_path);
    std::shared_ptr<Shader> get_shader(const std::string& name) const;
    std::shared_ptr<Shader> get_shader(uint32_t id) const;


    // Model Loading and Management
    uint32_t load_model(const std::shared_ptr<Model>& model);
    uint32_t load_model(const std::string& path);
    std::shared_ptr<Model> get_model(const std::string& path) const;

private:
    uint32_t _next_mat_id = 0;
    uint32_t _next_shader_id = 0;
    uint32_t _next_model_id = 1;
    uint32_t _next_tex_id = 0;

    std::shared_ptr<Shader> default_shader;
    std::shared_ptr<Texture2D> default_texture;
    Material default_material;

    std::unordered_map<std::string, uint32_t> _models_name_to_id;
    std::unordered_map<uint32_t, std::shared_ptr<Model>> _models;

    std::unordered_map<uint32_t, std::shared_ptr<Shader>> _shaders;
    std::unordered_map<std::string, uint32_t> _shaders_name_to_id;

    std::unordered_map<uint32_t, std::shared_ptr<Texture2D>> _textures_2d;
    std::unordered_map<std::string, uint32_t> _tex_path_to_id;

    std::unordered_map<uint32_t, Material> _materials;
    std::unordered_map<std::string, uint32_t> _materials_name_to_id;
};
