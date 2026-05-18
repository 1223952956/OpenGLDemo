#pragma once
#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "Shader.h"


struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture
{
    unsigned int Id;
    std::string Type;
};

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures, const std::string& name = "");
    
    // Needs a destructor to free VAO blabla, but I'm kinda lazy...
    
    void Draw(Shader& shader);

    const std::string& GetName() const { return Name; };
private:
    std::string Name;

    /*  Mesh Data  */
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    std::vector<Texture> Textures;

    /*  Rendering Data  */
    unsigned int VAO, VBO, EBO;

    void SetupMesh();
};

