#pragma once
#include <unordered_map>
#include <memory>

#include <assimp/scene.h>

#include "Texture.h"


class TextureManager
{
public:
	static Texture2D* Load(const std::string& path);
	static Texture2D* Load(const std::string& path, const aiScene* scene);

	static void ShutDown();

private:
	static std::unordered_map<std::string, std::unique_ptr<Texture2D>> Texture2DMap;

	static unsigned int CreateGLTexture(int width, int height, int nrChannels, unsigned char* data);
};

