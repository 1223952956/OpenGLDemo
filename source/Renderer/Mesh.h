#pragma once
#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "Shader.h"
#include "Material.h"


struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};


class Mesh
{
public:
    Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, Material* material, const std::string& name = "");
    ~Mesh();
    
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;
    
    void Draw(Shader* shader);

    const std::string& GetName() const { return Name; };
private:
    std::string Name;

    /*  Mesh Data  */
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;

    Material* MaterialPtr;

    /*  Rendering Data  */
    unsigned int VAO, VBO, EBO;

    void SetupMesh();

    void Release();
};

