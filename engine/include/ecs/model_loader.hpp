#pragma once

#include "ecs/components.hpp"
#include "core/resource_manager.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class ModelLoader {
public:
    Model load_to_gpu(const std::string& path, ResourceManager& resMgr);
private:
    Mesh upload_mesh(aiMesh* mesh, const aiScene* scene, const std::string& directory, ResourceManager& resMgr);
    void process_node(aiNode* node, const aiScene* scene, Model& container, const std::string& directory, ResourceManager& resMgr);
};
