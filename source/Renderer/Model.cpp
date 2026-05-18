#include "Model.h"
#include <stb_image.h>

void Model::Draw(Shader shader)
{
	for (unsigned int i = 0; i < Meshes.size(); i++)
		Meshes[i].Draw(shader);
}

void Model::Draw(Shader shader, const std::string& name)
{
	for (unsigned int i = 0; i < Meshes.size(); i++)
	{
		if (Meshes[i].GetName() == name)
		{
			Meshes[i].Draw(shader);
		}
	}
}

void Model::LoadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_JoinIdenticalVertices);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "[Model] ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	Directory = path.substr(0, path.find_last_of('/'));

	glm::mat4 rootTransform = glm::mat4(1.f);

	ProcessNode(scene->mRootNode, scene, rootTransform, 0);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, glm::mat4 parentTransform, int depth)
{
	for (int i = 0; i < depth; ++i)
	{
		std::cout << "    ";
	}
	std::cout << node->mName.C_Str() << '\n';

	glm::mat4 nodeTransform = ConvertMatrix(node->mTransformation);
	glm::mat4 globalTransform = parentTransform * nodeTransform;

	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		for (int i = 0; i < depth + 1; ++i)
		{
			std::cout << "    ";
		}
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Meshes.emplace_back(ProcessMesh(mesh, scene, globalTransform));
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene, globalTransform, depth + 1);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, glm::mat4 globalTransform)
{
	std::cout << "Mesh: " << mesh->mName.C_Str() << '\n';

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// Vertex
	vertices.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		
		// apply globalTransform from node
		vertex.Position = globalTransform * glm::vec4(ToGlm(mesh->mVertices[i]), 1.f);

		// Normal needs inverse and transpose
		glm::mat4 normalMatrix = glm::transpose(glm::inverse(glm::mat3(globalTransform)));
		vertex.Normal = normalMatrix * glm::vec4(ToGlm(mesh->mNormals[i]), 1.f);

		if (mesh->mTextureCoords[0])
			vertex.TexCoords = ToGlm(mesh->mTextureCoords[0][i]);
		else
			vertex.TexCoords = glm::vec2(0.f);

		vertices.emplace_back(vertex);
	}
	// Index
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.emplace_back(face.mIndices[j]);
		}
	}
	// Texture
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, scene,
			aiTextureType_DIFFUSE, "texture_diffuse");
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, scene,
			aiTextureType_SPECULAR, "texture_specular");

		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures, mesh->mName.C_Str());
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, const aiScene* scene, aiTextureType type, const std::string& typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString path;
		mat->GetTexture(type, i, &path);
		std::cout << "Texture: " << path.C_Str() << '\n';		

		auto it = TexturesLoaded.find(path.C_Str());
		if (it != TexturesLoaded.end())
		{
			textures.emplace_back(it->second);
		}
		else
		{
			Texture texture;
			if (path.C_Str()[0] == '*')
			{
				texture.Id = TextureFromMemory(path.C_Str(), scene);
			}
			else
			{
				texture.Id = TextureFromFile(path.C_Str(), Directory);
			}
			texture.Type = typeName;
			textures.emplace_back(texture);
			TexturesLoaded[path.C_Str()] = texture;
		}
	}
	return textures;
}

unsigned int Model::TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = directory + '/' + path;
	int width, height, nrChannels;

	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

	if (!data)
	{
		std::cerr << "[Model] Failed to load texture:" << path << std::endl;
		stbi_image_free(data);
		return -1;
	}

	unsigned int textureID = CreateGLTexture(width, height, nrChannels, data);

	stbi_image_free(data);

	return textureID;
}

unsigned int Model::TextureFromMemory(const char* path, const aiScene* scene)
{
	const aiTexture* texture = scene->GetEmbeddedTexture(path);
	int width, height, channels;

	unsigned char* data = stbi_load_from_memory(
		reinterpret_cast<unsigned char*>(texture->pcData),
		texture->mWidth,
		&width,
		&height,
		&channels,
		0
		);

	if (!data)
	{
		std::cerr << "[Model] Failed to load texture:" << path << std::endl;
		stbi_image_free(data);
		return -1;
	}

	unsigned int textureId = CreateGLTexture(width, height, channels, data);

	stbi_image_free(data);

	return textureId;
}

unsigned int Model::CreateGLTexture(int width, int height, int nrChannels, unsigned char* data)
{
	GLenum format;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;
	else
		format = GL_RGB;

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return textureID;
}

void Model::PrintNode(aiNode* node, int depth)
{
}
