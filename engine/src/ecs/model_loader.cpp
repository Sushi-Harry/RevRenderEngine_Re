#include "ecs/model_loader.hpp"
#include "renderer/material.hpp"

inline glm::mat4 aiMatrix4x4_to_glm_mat4(const aiMatrix4x4& from){
    glm::mat4 to;
    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
    to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
    return to;
}

Model ModelLoader::load_to_gpu(const std::string& path, ResourceManager& resMgr){
    Assimp::Importer importer;

    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_OptimizeMeshes);
    std::string directory = path.substr(0, path.find_last_of('/'));

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cerr << "ASSIMP_ERROR::" << importer.GetErrorString() << '\n';
        return {};
    }

    Model modelContainer;
    process_node(scene->mRootNode, scene, modelContainer, directory, resMgr, glm::mat4(1.0F));
    return modelContainer;
}

Mesh ModelLoader::upload_mesh(aiMesh* mesh, const aiScene* scene, const std::string& directory, ResourceManager& resMgr, glm::mat4 global_transform){
    std::vector<VertexComponent> vertices;
    std::vector<uint32_t> indices;

    for(int i = 0; i < mesh->mNumVertices; i++){
        VertexComponent vertex{};

        // Extracting the position data
        vertex._position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        // Extracting the normal data
        if(mesh->HasNormals())
            vertex._normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
        else
            vertex._normal = {0, 0, 0};
        // Extracting the texture coords
        if(mesh->mTextureCoords[0])
            vertex._tex_coords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
        else
            vertex._tex_coords = { 0, 0};

        vertices.push_back(vertex);
    }

    // Extracting indices data
    for(unsigned int i = 0; i < mesh->mNumFaces; i++){
        const aiFace& face = mesh->mFaces[i];
        for(int j = 0; j < face.mNumIndices; j++){
            indices.push_back(face.mIndices[j]);
        }
    }

    // Material data extraction
    Material mat;
    if(mesh->mMaterialIndex >= 0){
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // Extracting diffuse maps
        for(unsigned int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++){
            aiString str;
            material->GetTexture(aiTextureType_DIFFUSE, i, &str);
            std::string fullPath = directory + "/" + std::string(str.C_Str());
            mat._diffuse_textures.push_back(resMgr.load_texture(fullPath, REV_TEXTURE_TYPE::REV_DIFFUSE));
        }
        // Extracting specular maps
        for(unsigned int i = 0; i < material->GetTextureCount(aiTextureType_SPECULAR); i++){
            aiString str;
            material->GetTexture(aiTextureType_SPECULAR, i, &str);
            std::string fullPath = directory + "/" + std::string(str.C_Str());
            mat._specular_textures.push_back(resMgr.load_texture(fullPath, REV_TEXTURE_TYPE::REV_SPECULAR));
        }
    }

    // Now's the part of the model loader that's specific to this rewrite and different from last one
    std::shared_ptr<VertexBuffer> vbo = VertexBuffer::Create(vertices.data(), vertices.size()*sizeof(VertexComponent), BufferUsageType::STATIC);
    std::shared_ptr<VertexArray> vao = VertexArray::Create();

    BufferLayout layout = {
        {"aPosition", ShaderDataType::FLOAT3},
        {"aNormal", ShaderDataType::FLOAT3},
        {"aTexCoords", ShaderDataType::FLOAT2}
    };
    vbo->setLayout(layout);
    vao->addVertexBuffer(vbo);

    std::shared_ptr<ElementBuffer> ebo = ElementBuffer::Create(indices.data(), indices.size(), BufferUsageType::STATIC);
    vao->setElementBuffer(ebo);

    std::string mat_name = std::string(mesh->mName.C_Str()) + ("_material");
    uint32_t generated_mat_id = resMgr.load_material(mat_name, mat);
    return Mesh{ ._vert_array=vao, ._local_transform=global_transform, ._material_id=generated_mat_id };

}

void ModelLoader::process_node(aiNode* node, const aiScene* scene, Model& container, const std::string& directory, ResourceManager& resMgr, glm::mat4 parent_transform){
    glm::mat4 node_transform = aiMatrix4x4_to_glm_mat4(node->mTransformation);

    glm::mat4 global_transform = parent_transform * node_transform;

    for(unsigned int i = 0; i < node->mNumMeshes; i++){
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        container._meshes.push_back(upload_mesh(mesh, scene, directory, resMgr, global_transform));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        process_node(node->mChildren[i], scene, container, directory, resMgr, global_transform);
    }
}
