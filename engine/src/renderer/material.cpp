#include "renderer/material.hpp"

#include "core/resource_manager.hpp"

void Material::Apply(ResourceManager& res_mgr, std::shared_ptr<Shader> _shader) const {
    // NOT BINDING THE SHADER AT THE START OF THIS FUNCTION BECAUSE I'M ALREADY BINDING IT BEFORE THIS FUNCTION IS BEING USED
    // UNIFORM SETTING!!!!
    _shader->setFloat("u_Material._shininess", _shininess);
    auto texDiff = res_mgr.get_texture(_diffuse_texture);
    texDiff->bind(0);
    _shader->setInt("u_Material._texture_diffuse", 0);
    auto texSpec = res_mgr.get_texture(_specular_texture);
    texSpec->bind(1);
    _shader->setInt("u_Material._texture_specular", 1);
}
