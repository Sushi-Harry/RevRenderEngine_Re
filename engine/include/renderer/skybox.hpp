#pragma once

#include "renderer/vertex_array.hpp"

#include <memory>
#include <vector>
#include <string>

class ResourceManager;
class Shader;
class Camera3D;

class Skybox{
public:
    Skybox(ResourceManager& res_mgr, const std::string& name, const std::vector<std::string>& faces_paths);
    ~Skybox() = default;

    void draw(ResourceManager& res_mgr, const Camera3D& cam, const std::string& name = "skybox");

    std::string _name;
private:
    void draw_setup();
    std::shared_ptr<VertexArray> _vao;
    uint32_t _cubemap_id = 0;
};
