#include "TextureManager.h"

#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stb_image.h>

std::unordered_map<std::string, std::unique_ptr<Texture2D>>TextureManager::Texture2DMap;
GLuint TextureManager::WhiteTexture;
GLuint TextureManager::BlackTexture;
GLuint TextureManager::NormalTexture;

void TextureManager::Init()
{
	WhiteTexture = CreateSolidTexture(1.f, 1.f, 1.f, 1.f);
	BlackTexture = CreateSolidTexture(0.f, 0.f, 0.f, 1.f);
	NormalTexture = CreateSolidTexture(0.5f, 0.5f, 1.f, 1.f);
}

Texture2D* TextureManager::Load(const std::string& path, aiTextureType type)
{
	auto it = Texture2DMap.find(path);

	if (it != Texture2DMap.end())
	{
		return it->second.get();
	}

	int width, height, channels;

	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

	if (!data)
	{
		std::cerr << "[TextureManager] Failed to load texture: " << path << std::endl;
		stbi_image_free(data);
		return nullptr;
	}

	std::cout << "Load texture: " << path << std::endl;

	GLenum internalFormat = TextureManager::GetInternalFormat(channels, type);
	GLenum dataFormat = TextureManager::GetDataFormat(channels);

	// Create texture
	auto texture = std::make_unique<Texture2D>();
	Texture2DData dataToSet;
	dataToSet.Data = data;
	dataToSet.Width = width;
	dataToSet.Height = height;
	dataToSet.InternalFormat = internalFormat;
	dataToSet.DataFormat = dataFormat;
	dataToSet.DataType = GL_UNSIGNED_BYTE;
	dataToSet.WarpParam = GL_REPEAT;
	dataToSet.MinFilter = GL_LINEAR_MIPMAP_LINEAR;
	dataToSet.MagFilter = GL_LINEAR;
	dataToSet.GenerateMipmaps = true;
	dataToSet.DebugName = path;

	texture->SetData(dataToSet);


	stbi_image_free(data);

	Texture2D* ptr = texture.get();
	Texture2DMap[texture->GetDebugName()] = std::move(texture);

	return ptr;
}

Texture2D* TextureManager::Load(const std::string& texNum, const std::string& dictionary, const aiScene* scene, aiTextureType type)
{
	const aiTexture* tex = scene->GetEmbeddedTexture(texNum.c_str());

	std::string key = dictionary + "/" + tex->mFilename.C_Str();
	auto it = Texture2DMap.find(key);
	if (it != Texture2DMap.end())
	{
		return it->second.get();
	}

	int width, height, channels;

	unsigned char* data = stbi_load_from_memory(
		reinterpret_cast<unsigned char*>(tex->pcData),
		tex->mWidth,
		&width,
		&height,
		&channels,
		0
	);

	if (!data)
	{
		std::cerr << "[TextureManager] Failed to load texture: " << texNum << std::endl;
		stbi_image_free(data);
		return nullptr;
	}

	std::cout << "Load texture: " << tex->mFilename.C_Str() << std::endl;

	GLenum internalFormat = TextureManager::GetInternalFormat(channels, type);
	GLenum dataFormat = TextureManager::GetDataFormat(channels);

	// Create texture
	auto texture = std::make_unique<Texture2D>();
	Texture2DData dataToSet;
	dataToSet.Data = data;
	dataToSet.Width = width;
	dataToSet.Height = height;
	dataToSet.InternalFormat = internalFormat;
	dataToSet.DataFormat = dataFormat;
	dataToSet.DataType = GL_UNSIGNED_BYTE;
	dataToSet.WarpParam = GL_REPEAT;	
	dataToSet.MinFilter = GL_LINEAR_MIPMAP_LINEAR;
	dataToSet.MagFilter = GL_LINEAR;
	dataToSet.GenerateMipmaps = true;
	dataToSet.DebugName = dictionary + "/" + tex->mFilename.C_Str();
	
	texture->SetData(dataToSet);


	stbi_image_free(data);

	Texture2D* ptr = texture.get();
	Texture2DMap[texture->GetDebugName()] = std::move(texture);

	return ptr;
}

Texture2D* TextureManager::LoadEquirectangularMap(const std::string& path)
{
	auto it = Texture2DMap.find(path);

	if (it != Texture2DMap.end())
	{
		return it->second.get();
	}

	stbi_set_flip_vertically_on_load(true);
	int width, height, channels;
	float* data = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
	if (!data)
	{
		std::cerr << "[TextureManager] Failed to load texture: " << path << std::endl;
		stbi_image_free(data);
		return nullptr;
	}

	std::cout << "Load texture: " << path << std::endl;

	auto texture = std::make_unique<Texture2D>();
	Texture2DData dataToSet;
	dataToSet.Data = data;
	dataToSet.Width = width;
	dataToSet.Height = height;
	dataToSet.InternalFormat = GL_RGB16F;
	dataToSet.DataFormat = GL_RGB;
	dataToSet.DataType = GL_FLOAT;
	dataToSet.WarpParam = GL_CLAMP_TO_EDGE;
	dataToSet.MinFilter = GL_LINEAR;
	dataToSet.MagFilter = GL_LINEAR;
	dataToSet.GenerateMipmaps = false;

	texture->SetData(dataToSet);

	stbi_image_free(data);

	Texture2D* ptr = texture.get();
	Texture2DMap[texture->GetDebugName()] = std::move(texture);

	return ptr;
}

void TextureManager::ShutDown()
{
	Texture2DMap.clear();
}

GLenum TextureManager::GetInternalFormat(int channels, aiTextureType type)
{
	GLenum internalFormat;
	if (channels == 1)
	{
		internalFormat = GL_R8;
	}
	else if (channels == 3)
	{
		internalFormat = GL_RGB8;
	}
	else if (channels == 4)
	{
		internalFormat = GL_RGBA8;
	}
	else
	{
		internalFormat = GL_RGBA8;
	}
	if (type == aiTextureType::aiTextureType_BASE_COLOR ||
		type == aiTextureType::aiTextureType_EMISSIVE)
	{
		internalFormat = (channels == 4) ? GL_SRGB8_ALPHA8 : GL_SRGB8;
	}
	return internalFormat;
}

GLenum TextureManager::GetDataFormat(int channels)
{
	GLenum dataFormat;
	if (channels == 1)
	{
		dataFormat = GL_RED;
	}
	else if (channels == 3)
	{
		dataFormat = GL_RGB;
	}
	else if (channels == 4)
	{
		dataFormat = GL_RGBA;
	}
	else
	{
		dataFormat = GL_RGBA;
	}
	return dataFormat;
}

GLuint TextureManager::CreateSolidTexture(float r, float g, float b, float a)
{
	unsigned char pixel[4];
	pixel[0] = static_cast<unsigned char>(glm::clamp(r, 0.0f, 1.0f) * 255.0f);
	pixel[1] = static_cast<unsigned char>(glm::clamp(g, 0.0f, 1.0f) * 255.0f);
	pixel[2] = static_cast<unsigned char>(glm::clamp(b, 0.0f, 1.0f) * 255.0f);
	pixel[3] = static_cast<unsigned char>(glm::clamp(a, 0.0f, 1.0f) * 255.0f);

	GLuint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texId;
}
