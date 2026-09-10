#include "Mesh.h"

#include <glad/glad.h>

Mesh::Mesh(std::vector<Vertex>& vertices,std::vector<unsigned int>& indices, Material* material, const std::string& name)
	:Vertices(std::move(vertices))
	,Indices(std::move(indices))
	,MaterialPtr(material)
	,Name(name)
	,VAO(0)
	,VBO(0)
	,EBO(0)
{
	SetupMesh();
}

Mesh::~Mesh()
{
	Release();
}

Mesh::Mesh(Mesh&& other) noexcept
	: Name(std::move(other.Name))
	, Vertices(std::move(other.Vertices))
	, Indices(std::move(other.Indices))
	, MaterialPtr(std::exchange(other.MaterialPtr, nullptr))
	, VAO(std::exchange(other.VAO, 0))
	, VBO(std::exchange(other.VBO, 0))
	, EBO(std::exchange(other.EBO, 0))
{
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
	if (this != &other)
	{
		Release();

		Name = std::move(other.Name);
		Vertices = std::move(other.Vertices);
		Indices = std::move(other.Indices);
		MaterialPtr = std::exchange(other.MaterialPtr, nullptr);
		VAO = std::exchange(other.VAO, 0);
		VBO = std::exchange(other.VBO, 0);
		EBO = std::exchange(other.EBO, 0);
	}

	return *this;
}

void Mesh::Draw(Shader* shader)
{
	MaterialPtr->Bind(shader);

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

void Mesh::Release()
{
	if (EBO != 0)
		glDeleteBuffers(1, &EBO);

	if (VBO != 0)
		glDeleteBuffers(1, &VBO);

	if (VAO != 0)
		glDeleteVertexArrays(1, &VAO);

	VAO = 0;
	VBO = 0;
	EBO = 0;
}
