#pragma once

#include "renderer/cubemap.hpp"
#include "renderer/shader.hpp"
#include "ecs/components.hpp"
#include "renderer/texture.hpp"
#include "renderer/material.hpp"

#include <unordered_map>



class Material;

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
    uint32_t load_material(const std::string& name, uint32_t diffuse, uint32_t specular, std::shared_ptr<Shader> shader);
    const Material& get_material(uint32_t) const;

    // Shader Loading and Management
    uint32_t load_shader(const std::string& name, const std::shared_ptr<Shader>& shader);
    uint32_t load_shader(const std::string& name, const std::string& v_path, const std::string& f_path);
    std::shared_ptr<Shader> get_shader(const std::string& name) const;
    std::shared_ptr<Shader> get_shader(uint32_t id) const;


    // Model Loading and Management
    uint32_t load_model(const Model& model);
    uint32_t load_model(const std::string& path);
    const Model& get_model(const std::string& path) const;
    const Model& get_model(uint32_t id) const;

    // Cubemap Management
    uint32_t load_cubemap(const std::string& name, std::shared_ptr<Cubemap> cube_map);
    uint32_t load_cubemap(const std::string& name, const std::vector<std::string>& path);
    std::shared_ptr<Cubemap> get_cubemap(const std::string& name);
    std::shared_ptr<Cubemap> get_cubemap(uint32_t id);

private:

    // These values start from 0 and increase as the respective objects in the unordered maps increase in number.
    // 0 = default / debugging materials, shaders, models and textures
    uint32_t _next_mat_id = 0;
    uint32_t _next_shader_id = 0;
    uint32_t _next_model_id = 0;
    uint32_t _next_tex_id = 0;
    uint32_t _next_cmap_id = 0;

    std::shared_ptr<Shader> default_shader;
    std::shared_ptr<Texture2D> default_texture;
    // Deleted the default material cause I don't need it to be present in this class since it's guaranteed to be stored at id = 0 in the respective map.
    Model default_model;

    // MODEL CACHE
    std::unordered_map<std::string, uint32_t> _models_path_to_id;
    std::unordered_map<uint32_t, Model> _models;

    // SHADER CACHE
    std::unordered_map<uint32_t, std::shared_ptr<Shader>> _shaders;
    std::unordered_map<std::string, uint32_t> _shaders_name_to_id;

    // TEXTURE CACHE
    std::unordered_map<uint32_t, std::shared_ptr<Texture2D>> _textures_2d;
    std::unordered_map<std::string, uint32_t> _tex_path_to_id;

    // MATERIAL CACHE
    std::unordered_map<uint32_t, Material> _materials;
    std::unordered_map<std::string, uint32_t> _materials_name_to_id;

    // CUBEMAP CACHE
    std::unordered_map<uint32_t, std::shared_ptr<Cubemap>> _cubemaps;
    std::unordered_map<std::string, uint32_t> _cubemaps_name_to_id;
};
