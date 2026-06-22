#include "core/resource_manager.hpp"

#include <utility>
#include "ecs/model_loader.hpp"
#include "renderer/material.hpp"

void ResourceManager::Init(){

    std::vector<std::string> default_faces = {
        "revrender/assets/skybox/basicDay/right.jpg",
        "revrender/assets/skybox/basicDay/left.jpg",
        "revrender/assets/skybox/basicDay/top.jpg",
        "revrender/assets/skybox/basicDay/bottom.jpg",
        "revrender/assets/skybox/basicDay/back.jpg",
        "revrender/assets/skybox/basicDay/front.jpg"
    };

    auto default_shader = Shader::Create("default_shader", "revrender/assets/core/default_shader.vert", "revrender/assets/core/default_shader.frag");
    load_shader(default_shader->getName(), default_shader);

    // This is something new.
    // =====.        \     /
    //  |   |         \   /
    //  |   |          \ /
    // =====' EFAULT    =  ALUES
    //
    // If you want to access DEFAULT DIFFUSE TEXTURE, use resource_manager.getTexture(0) or resource_manager.getTexture("default_diffuse");
    // If you want to access DEFAULT SPECULAR TEXTURE, use resource_manager.getTexture(1) or resource_manager.getTexture("default_specular");
    // If you want to access DEFAULT NORMAL TEXTURE, use resource_manager.getTexture(2) or resource_manager.getTexture("default_normal");
    _next_tex_id = 0;
    auto default_texture_diffuse = Texture2D::CreateDefault(REV_TEXTURE_TYPE::REV_DIFFUSE);
    _textures_2d[_next_tex_id] = default_texture_diffuse;
    _tex_path_to_id["default_diffuse"] = _next_tex_id++;
    auto default_texture_specular = Texture2D::CreateDefault(REV_TEXTURE_TYPE::REV_SPECULAR);
    _textures_2d[_next_tex_id] = default_texture_specular;
    _tex_path_to_id["default_specular"] = _next_tex_id++;
    auto default_texture_normal = Texture2D::CreateDefault(REV_TEXTURE_TYPE::REV_NORMAL);
    _textures_2d[_next_tex_id] = default_texture_normal;
    _tex_path_to_id["default_normal"] = _next_tex_id++;

    Material default_material;
    default_material._diffuse_texture = 0;
    default_material._specular_texture = 1;
    default_material._normal_texture = 2;
    default_material._shininess = 32.0F;
    load_material("default_material", default_material);

    // Default model loading
    Model default_model = {._meshes={}, ._path={}};
    load_model(default_model);

    // Skybox shader loaded with all the other default shaders
    load_shader("skybox", "revrender/assets/core/skybox.vert", "revrender/assets/core/skybox.frag");

    // Shadow Map Shader
    load_shader("shadow_map_shader", "revrender/assets/core/default_shadow_map.vert", "revrender/assets/core/default_shadow_map.frag");

    // Post Processing shaders preloading
    // Chromatic aberration shader
    load_shader("chromatic_aberration", "revrender/assets/core/postprocessing_shader/chromatic_aberration.vert", "revrender/assets/core/postprocessing_shader/chromatic_aberration.frag");
    // Pixelation Shader
    load_shader("pixelation", "revrender/assets/core/postprocessing_shader/pixelation.vert", "revrender/assets/core/postprocessing_shader/pixelation.frag");
    // CRT Shader
    load_shader("crt", "revrender/assets/core/postprocessing_shader/crt.vert", "revrender/assets/core/postprocessing_shader/crt.frag");
    // Film Grain shader
    load_shader("filmgrain", "revrender/assets/core/postprocessing_shader/filmgrain.vert", "revrender/assets/core/postprocessing_shader/filmgrain.frag");
    // Fxaa shader
    load_shader("fxaa", "revrender/assets/core/postprocessing_shader/fxaa.vert", "revrender/assets/core/postprocessing_shader/fxaa.frag");

    // Loading the shader for G-Buffer
    load_shader("geometry_buffer", "revrender/assets/core/default_gshader.vert", "revrender/assets/core/default_gshader.frag");
    // Loading the shader for deferred lighting
    load_shader("default_shader_deferred", "revrender/assets/core/default_lit_shader_deferred.vert", "revrender/assets/core/default_lit_shader_deferred.frag");

    // Loading the default cubemap
    load_cubemap("skybox", std::make_shared<Cubemap>(default_faces));
}


// =========         ||
// ||                ||
// =========         ||
//        ||         ||
// ========= HADER   ======== LOADING
uint32_t ResourceManager::load_shader(const std::string& name, const std::shared_ptr<Shader>& shader){
    if(_shaders_name_to_id.contains(name)){
        return _shaders_name_to_id.at(name);
    }

    uint32_t id = _next_shader_id++;
    _shaders[id] = shader;
    _shaders_name_to_id[name] = id;
    return id;
}

uint32_t ResourceManager::load_shader(const std::string& name, const std::string& v_path, const std::string& f_path){
    if(_shaders_name_to_id.contains(name)){
        return _shaders_name_to_id.at(name);
    }

    uint32_t id = _next_shader_id++;
    _shaders[id] = Shader::Create(name, v_path, f_path);
    _shaders_name_to_id[name] = id;
    return id;
}

std::shared_ptr<Shader> ResourceManager::get_shader(const std::string& name) const {
    if(_shaders_name_to_id.contains(name)){
        return _shaders.at(_shaders_name_to_id.at(name));
    }
    return _shaders.at(0);
}

std::shared_ptr<Shader> ResourceManager::get_shader(uint32_t id) const {
    if(_shaders.contains(id)){
        return _shaders.at(id);
    }
    return _shaders.at(0);
}


//=========      ||
//   ||          ||
//   ||          ||
//   ||          ||
//   || EXTURE   ======== OADING
uint32_t ResourceManager::load_texture(const std::string& path, REV_TEXTURE_TYPE type){
    if(_tex_path_to_id.contains(path)){
        return _tex_path_to_id[path];
    }

    uint32_t id = _next_tex_id++;
    _textures_2d[id] = Texture2D::Create(path);
    _tex_path_to_id[path] = id;

    return id;
}

std::shared_ptr<Texture2D> ResourceManager::get_texture(const std::string& path) const {
    if(_tex_path_to_id.contains(path)){
        // This is definitely one of the most unnecessarily complex ways to write this.
        // return _textures_2d.find(_tex_path_to_id.find(path)->second)->second; // Commenting this line cause there's a better way
        return _textures_2d.at(_tex_path_to_id.at(path));
    }
    return _textures_2d.at(0);
}

uint32_t ResourceManager::load_texture(const std::shared_ptr<Texture2D>& tex){
    if(_tex_path_to_id.contains(tex->getPath())){
        return _tex_path_to_id.at(tex->getPath());
    }
    uint32_t id = _next_tex_id++;
    _textures_2d[id] = tex;
    _tex_path_to_id[tex->getPath()] = id;
    return id;
}

std::shared_ptr<Texture2D> ResourceManager::get_texture(uint32_t id) const {
    if(_textures_2d.contains(id)){
        return _textures_2d.at(id);
    }
    return _textures_2d.at(0);
}


// |\    /|          ||
// |\\  //|          ||
// ||\\//||          ||
// ||    ||          ||
// ||    || ATERIAL  ======== OADING
uint32_t ResourceManager::load_material(const std::string& name, const Material& mat){
    if(_materials_name_to_id.contains(name)){
        return _materials_name_to_id.at(name);
    }
    uint32_t id = _next_mat_id++;
    _materials[id] = mat;
    _materials_name_to_id[name] = id;
    return id;
}

uint32_t ResourceManager::load_material(const std::string& name, uint32_t diffuse, uint32_t specular, std::shared_ptr<Shader> shader){
    if(_materials_name_to_id.contains(name)){
        return _materials_name_to_id.at(name);
    }
    uint32_t id = _next_mat_id++;

    Material new_mat;
    new_mat._diffuse_texture = diffuse;
    new_mat._specular_texture = specular;
    new_mat._normal_texture = 2; // DEFAULT NORMAL TEXTURE EXISTS AT [texture id = 2]
    _materials[id] = new_mat;
    _materials_name_to_id[name] = id;

    return id;
}

const Material& ResourceManager::get_material(uint32_t id) const {
    if(_materials.contains(id)){
        return _materials.at(id);
    }
    return _materials.at(0);
}


// |\    /|       ||
// |\\  //|       ||
// ||\\//||       ||
// || \/ ||       ||
// ||    || ODEL  ======== OADING
uint32_t ResourceManager::load_model(const Model& model){
    if(_models_path_to_id.contains(model._path)){
        return _models_path_to_id.at(model._path);
    }
    uint32_t id = _next_model_id++;
    _models[id] = model;
    _models_path_to_id[model._path] = id;
    return _models_path_to_id.at(model._path);
}

uint32_t ResourceManager::load_model(const std::string& path){
    if(_models_path_to_id.contains(path)){
        return _models_path_to_id.at(path);
    }
    uint32_t id = _next_model_id++;
    _models[id] = ModelLoader::load_to_gpu(path, *this);
    _models_path_to_id[path] = id;
    return id;
}

const Model& ResourceManager::get_model(const std::string& path) const{
    if(_models_path_to_id.contains(path)){
        return _models.at(_models_path_to_id.at(path));
    }
    return _models.at(0);
}

const Model& ResourceManager::get_model(uint32_t id) const {
    if(_models.contains(id)){
        return _models.at(id);
    }
    return _models.at(0);
}

// =========         ||
// ||                ||
// ||                ||
// ||                ||
// ========= UBEMAP  ======== OADING
uint32_t ResourceManager::load_cubemap(const std::string& name, std::shared_ptr<Cubemap> cube_map){
    if(_cubemaps_name_to_id.contains(name)){
        return _cubemaps_name_to_id.at(name);
    }
    uint32_t id = _next_cmap_id++;
    _cubemaps[id] = std::move(cube_map);
    _cubemaps_name_to_id[name] = id;
    return id;
}

uint32_t ResourceManager::load_cubemap(const std::string& name, const std::vector<std::string>& faces_path){
    if(_cubemaps_name_to_id.contains(name)){
        return _cubemaps_name_to_id.at(name);
    }
    uint32_t id = _next_cmap_id++;
    _cubemaps[id] = std::make_shared<Cubemap>(faces_path);
    _cubemaps_name_to_id[name] = id;
    return id;
}

std::shared_ptr<Cubemap> ResourceManager::get_cubemap(const std::string& name){
    if(_cubemaps_name_to_id.contains(name)){
        return _cubemaps.at(_cubemaps_name_to_id.at(name));
    }
    return _cubemaps.at(0);
}

std::shared_ptr<Cubemap> ResourceManager::get_cubemap(uint32_t id){
    if(_cubemaps.contains(id)){
        return _cubemaps.at(id);
    }
    return _cubemaps.at(0);
}
