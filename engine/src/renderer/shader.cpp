#include "renderer/shader.hpp"

void ShaderLoader::addShader(const std::string& name, const std::shared_ptr<Shader>& shader){
    if(!_shader_cache.contains(name)){
        _shader_cache[name] = shader;
    }
}

void ShaderLoader::addShader(const std::string& name, const std::string& v_path, const std::string& f_path){
    if(!_shader_cache.contains(name)){
        _shader_cache[name] = Shader::Create(name, v_path, f_path);
    }
}

std::shared_ptr<Shader> ShaderLoader::getShader(const std::string& name) const {
    auto it = _shader_cache.find(name);
    if(it != _shader_cache.end()){
        return it->second;
    }
    return nullptr;
}
