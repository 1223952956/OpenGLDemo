#include "TextureManager.h"

#include <iostream>

#include <glad/glad.h>
#include <stb_image.h>

std::unordered_map<std::string, std::unique_ptr<Texture2D>>TextureManager::Texture2DMap;

Texture2D* TextureManager::Load(const std::string& path)
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

	texture->Id = CreateGLTexture(width, height, channels, data);
	texture->Path = path;

	stbi_image_free(data);

	Texture2D* ptr = texture.get();
	Texture2DMap[texture->Path] = std::move(texture);

	return ptr;
}

Texture2D* TextureManager::Load(const std::string& path, const aiScene* scene)
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

	texture->Id = CreateGLTexture(width, height, channels, data);
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

unsigned int TextureManager::CreateGLTexture(int width, int height, int nrChannels, unsigned char* data)
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
