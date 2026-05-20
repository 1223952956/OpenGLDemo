#include "TextureManager.h"

#include <iostream>

#include <glad/glad.h>
#include <stb_image.h>

std::unordered_map<std::string, std::unique_ptr<Texture2D>>TextureManager::Texture2DMap;

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
		std::cerr << "[Model] Failed to load texture:" << path << std::endl;
		stbi_image_free(data);
		return nullptr;
	}

	auto texture = std::make_unique<Texture2D>();

	texture->Id = CreateGLTexture(width, height, channels, data, type);
	texture->Path = path;

	stbi_image_free(data);

	Texture2D* ptr = texture.get();
	Texture2DMap[texture->Path] = std::move(texture);

	return ptr;
}

Texture2D* TextureManager::Load(const std::string& path, const aiScene* scene, aiTextureType type)
{
	auto it = Texture2DMap.find(path);

	if (it != Texture2DMap.end())
	{
		return it->second.get();
	}

	const aiTexture* tex = scene->GetEmbeddedTexture(path.c_str());
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
		std::cerr << "[Model] Failed to load texture:" << path << std::endl;
		stbi_image_free(data);
		return nullptr;
	}

	auto texture = std::make_unique<Texture2D>();

	texture->Id = CreateGLTexture(width, height, channels, data, type);
	texture->Path = path;

	stbi_image_free(data);

	Texture2D* ptr = texture.get();
	Texture2DMap[texture->Path] = std::move(texture);

	return ptr;
}

void TextureManager::ShutDown()
{
	Texture2DMap.clear();
}

unsigned int TextureManager::CreateGLTexture(int width, int height, int nrChannels, unsigned char* data, aiTextureType type)
{
	GLenum internalFormat;
	GLenum dataFormat;

	if (nrChannels == 1)
	{
		internalFormat = GL_R8;
		dataFormat = GL_RED;
	}
	else if (nrChannels == 3)
	{
		internalFormat = GL_RGB8;
		dataFormat = GL_RGB;
	}
	else if (nrChannels == 4)
	{
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	}
	else
	{
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	}

	if (type == aiTextureType::aiTextureType_BASE_COLOR)
	{
		internalFormat = (nrChannels == 4) ? GL_SRGB8_ALPHA8 : GL_SRGB8;
	}

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;
}
