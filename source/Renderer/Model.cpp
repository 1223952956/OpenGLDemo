#include "Model.h"
#include <stb_image.h>
#include "spdlog/spdlog.h"

#include "TextureManager.h"

void Model::Draw(Shader* shader)
{
	for (unsigned int i = 0; i < Meshes.size(); i++)
		Meshes[i].Draw(shader);
}

void Model::Draw(Shader* shader, const std::string& name)
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
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate |aiProcess_FlipUVs);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		spdlog::error("[Model] ASSIMP::{}", importer.GetErrorString());
		return;
	}
	Directory = path.substr(0, path.find_last_of('/'));

	glm::mat4 rootTransform = glm::mat4(1.f);

	LoadMaterials(scene);

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

	// Normal needs inverse and transpose
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(globalTransform)));

	// Vertex
	vertices.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		
		// apply globalTransform from node
		vertex.Position = glm::vec3(globalTransform * glm::vec4(ToGlm(mesh->mVertices[i]), 1.f));

		// apply normalMatrix
		vertex.Normal = glm::normalize(normalMatrix * ToGlm(mesh->mNormals[i]));

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
	// Material
	Material* matPtr = Materials[mesh->mMaterialIndex].get();

	return Mesh(vertices, indices, matPtr, mesh->mName.C_Str());
}

void Model::LoadMaterials(const aiScene* scene)
{
	Materials.reserve(scene->mNumMaterials);

	for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		aiMaterial* aiMat = scene->mMaterials[i];

		aiString name;
		scene->mMaterials[i]->Get(AI_MATKEY_NAME, name);
		printf("Load Material[%d]: '%s'\n", i, name.C_Str());

		auto material = std::make_unique<Material>();

		// Base Color
		material->BaseColorTexture =
			LoadTexture(aiMat, aiTextureType_BASE_COLOR, scene);
		// Normal
		material->NormalTexture =
			LoadTexture(aiMat, aiTextureType_NORMALS, scene);

		// Metallic Roughness
		material->MetallicRoughnessTexture =
			LoadTexture(aiMat, aiTextureType_GLTF_METALLIC_ROUGHNESS, scene);
		float metallic = 1.0f;
		if (aiMat->Get(AI_MATKEY_METALLIC_FACTOR, metallic) == AI_SUCCESS)
		{
			//printf("metallicFactor: %f\n", metallic);
			material->MetallicFactor = metallic;
		}
		float roughness = 1.0f;
		if (aiMat->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness) == AI_SUCCESS)
		{
			//printf("roughnessFactor: %f\n", roughness);
			material->RoughnessFactor = roughness;
		}

		// Emissive
		material->EmissiveTexture =
			LoadTexture(aiMat, aiTextureType_EMISSIVE, scene);
		// Ambient Occlusion
		material->OcclusionTexture =
			LoadTexture(aiMat, aiTextureType_LIGHTMAP, scene);

		Materials.emplace_back(std::move(material));
	}
}

Texture2D* Model::LoadTexture(aiMaterial* mat, aiTextureType type, const aiScene* scene)
{
	if (mat->GetTextureCount(type) == 0)
	{
		return nullptr;
	}

	aiString str;
	mat->GetTexture(type, 0, &str);

	if (str.C_Str()[0] == '*')
	{
		return TextureManager::Load(str.C_Str(), Directory, scene, type);
	}
	else
	{
		std::string path = Directory + "/" + str.C_Str();
		return TextureManager::Load(path, type);
	}
}

void Model::PrintNode(aiNode* node, int depth)
{
}
