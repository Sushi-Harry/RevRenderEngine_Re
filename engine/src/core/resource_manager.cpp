#include "core/resource_manager.hpp"
#include "ecs/model_loader.hpp"

void ResourceManager::Init(){
    default_shader = Shader::Create("default_shader", "revrender/assets/core/default_shader.vert", "revrender/assets/core/default_shader.frag");
    load_shader(default_shader->getName(), default_shader);
    default_texture = Texture2D::CreateDefault();
    load_texture(default_texture);
    // Initialized every single vector to just a vector with a single value (0) because 0 basically means it's a default white pixel texture.
    default_material = {._diffuse_textures={0}, ._specular_textures={0}, ._normal_textures={0}};
    load_material("default_material", default_material);
    default_model = {{}, {}};
    load_model(default_model);
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
    return default_shader;
}

std::shared_ptr<Shader> ResourceManager::get_shader(uint32_t id) const {
    if(_shaders.contains(id)){
        return _shaders.at(id);
    }
    return default_shader;
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
    return default_texture;
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
    return default_texture;
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

uint32_t ResourceManager::load_material(const std::string& name, const std::vector<uint32_t>& diffuse, const std::vector<uint32_t>& specular){
    if(_materials_name_to_id.contains(name)){
        return _materials_name_to_id.at(name);
    }
    uint32_t id = _next_mat_id++;
    _materials[id] = {._diffuse_textures=diffuse, ._specular_textures=specular, ._normal_textures={0}};
    _materials_name_to_id[name] = id;
    return id;
}

const Material& ResourceManager::get_material(uint32_t id) const {
    if(_materials.contains(id)){
        return _materials.at(id);
    }
    return default_material;
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
    // This reminds me that
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
    return default_model;
}

const Model& ResourceManager::get_model(uint32_t id) const {
    if(_models.contains(id)){
        return _models.at(id);
    }
    return default_model;
}
