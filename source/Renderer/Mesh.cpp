#include "Mesh.h"
#include <glad/glad.h>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures, const std::string& name)
	:Vertices(vertices)
	,Indices(indices)
	,Textures(textures)
	,Name(name)
	,VAO(0)
	,VBO(0)
	,EBO(0)
{
	SetupMesh();
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseN = 0;
	unsigned int specularN = 0;

	for (unsigned int i = 0; i < Textures.size(); ++i)
	{
		unsigned int number = 0;
		std::string type = Textures[i].Type;

		if (type == "texture_diffuse")
		{
			++diffuseN;
			number = diffuseN;
		}
		else if (type == "texture_specular")
		{
			++specularN;
			number = specularN;
		}
		else
		{
			std::cerr << "[Mesh] Undefined texture type: "<< type << std::endl;
			return;
		}
			
		shader.setInt(("material." + type + std::to_string(number)), i);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, Textures[i].Id);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// Normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// TexCoords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}
