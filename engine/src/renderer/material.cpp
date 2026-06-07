#include "renderer/material.hpp"

#include "core/resource_manager.hpp"

void Material::Apply(ResourceManager& res_mgr, std::shared_ptr<Shader> _shader) const {
    // NOT BINDING THE SHADER AT THE START OF THIS FUNCTION BECAUSE I'M ALREADY BINDING IT BEFORE THIS FUNCTION IS BEING USED
    // UNIFORM SETTING!!!!
    _shader->setFloat("u_Material._shininess", _shininess);
    int tex_slot = 0;
    if(!_diffuse_textures.empty() || !_specular_textures.empty()){
        if(!_diffuse_textures.empty()){
            for(int i = 0; i < _diffuse_textures.size(); i++){
                auto tex = res_mgr.get_texture(_diffuse_textures[i]);
                tex->bind(tex_slot);
                _shader->setInt("u_Material._texture_diffuse", tex_slot);
                tex_slot++;
            }
        }
        if(!_specular_textures.empty()){
            for(int i = 0; i < _specular_textures.size(); i++){
                auto tex = res_mgr.get_texture(_specular_textures[i]);
                tex->bind(tex_slot);
                _shader->setInt("u_Material._texture_specular", tex_slot);
                tex_slot++;
            }
        }
    }
}
