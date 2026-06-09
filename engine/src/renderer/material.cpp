#include "renderer/material.hpp"

#include "core/resource_manager.hpp"

void Material::Apply(ResourceManager& res_mgr, std::shared_ptr<Shader> _shader) const {
    // NOT BINDING THE SHADER AT THE START OF THIS FUNCTION BECAUSE I'M ALREADY BINDING IT BEFORE THIS FUNCTION IS BEING USED
    // UNIFORM SETTING!!!!
    _shader->setFloat("u_Material._shininess", _shininess);
    if(_diffuse_texture != 0){
        auto tex = res_mgr.get_texture(_diffuse_texture);
        tex->bind(0);
        _shader->setInt("u_Material._texture_diffuse", 0);
    }
    if(_specular_texture != 0){
        auto tex = res_mgr.get_texture(_specular_texture);
        tex->bind(1);
        _shader->setInt("u_Material._texture_specular", 1);
    }
}
