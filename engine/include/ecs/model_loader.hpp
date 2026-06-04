#pragma once

#include "ecs/components.hpp"
#include "core/resource_manager.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class ModelLoader {
public:
    static Model load_to_gpu(const std::string& path, ResourceManager& resMgr);
private:
    static Mesh upload_mesh(aiMesh* mesh, const aiScene* scene, const std::string& directory, ResourceManager& resMgr, glm::mat4 global_transform);
    static void process_node(aiNode* node, const aiScene* scene, Model& container, const std::string& directory, ResourceManager& resMgr, glm::mat4 parent_transform);
};
