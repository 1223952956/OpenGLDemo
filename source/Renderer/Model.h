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
    void Draw(Shader shader);
    void Draw(Shader shader, const std::string& name);

private:
    std::vector<Mesh> Meshes;
    std::string Directory;
    std::unordered_map<std::string, Texture> TexturesLoaded;

    void LoadModel(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene, int depth);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, const aiScene* scene, aiTextureType type,
        const std::string& typeName);

    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
    unsigned int TextureFromMemory(const char* path, const aiScene* scene);
    unsigned int CreateGLTexture(int width, int height, int nrChannels, unsigned char* data);

    void PrintNode(aiNode* node, int depth = 0);

    inline glm::vec3 ToGlm(const aiVector3D& v)
    {
        return { v.x, v.y, v.z };
    }

    inline glm::vec2 ToGlm(const aiVector2D& v)
    {
        return { v.x, v.y };
    }
};

