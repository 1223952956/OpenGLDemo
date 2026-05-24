#pragma once
#include <unordered_map>
#include <memory>

#include <assimp/scene.h>

#include "Texture.h"

class TextureManager
{
public:
	static void Init();

	// Now just for loading Equirectangular Map
	static Texture2D* Load(const std::string& path);

	static Texture2D* Load(const std::string& path, aiTextureType type);
	static Texture2D* Load(const std::string& texNum, const std::string& dictionary, const aiScene* scene, aiTextureType type);


	static GLuint GetWhiteTexture() { return WhiteTexture; };
	static GLuint GetBlackTexture() { return BlackTexture; };
 	static GLuint GetNormalTexture() { return NormalTexture; };

	static void ShutDown();

private:
	static GLuint WhiteTexture;
	static GLuint BlackTexture;
	static GLuint NormalTexture;

	static std::unordered_map<std::string, std::unique_ptr<Texture2D>> Texture2DMap;

	static unsigned int CreateGLTexture(int width, int height, int nrChannels, unsigned char* data, aiTextureType type);
	// Now just for creating Equirectangular Map
	static unsigned int CreateGLTexture(int width, int height, int nrChannels, float* data);

	static GLuint CreateSolidTexture(float r, float g, float b, float a);
};

