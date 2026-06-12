#pragma once
#include <vector>
#include <unordered_map>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"

class Model
{
public:
    Model(const char* path)
    {
        LoadModel(path);
    }
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;
    Model(Model&&) noexcept = default;
    Model& operator=(Model&&) noexcept = default;

    void Draw(Shader* shader);
    void Draw(Shader* shader, const std::string& name);

private:
    std::vector<Mesh> Meshes;
    std::vector<std::unique_ptr<Material>> Materials;
    std::string Directory;

    void LoadModel(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene, glm::mat4 parentTransform, int depth);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, glm::mat4 globalTransform);

    void LoadMaterials(const aiScene* scene);
    Texture2D* LoadTexture(aiMaterial* mat, aiTextureType type, const aiScene* scene);

    void PrintNode(aiNode* node, int depth = 0);

    inline glm::mat4 ConvertMatrix(const aiMatrix4x4& from)
    {
        glm::mat4 to;

        to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
        to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
        to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
        to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;

        return to;
    }

    inline glm::vec3 ToGlm(const aiVector3D& v)
    {
        return { v.x, v.y, v.z };
    }

    inline glm::vec2 ToGlm(const aiVector2D& v)
    {
        return { v.x, v.y };
    }
};

