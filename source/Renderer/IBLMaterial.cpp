#include "IBLMaterial.h"

IBLMaterial::IBLMaterial()
{
	EnvCubeMap = std::make_unique<Cubemap>();
	IrradianceMap = std::make_unique<Cubemap>();
	PrefilterMap = std::make_unique<Cubemap>();
	BRDFLUT = std::make_unique<Texture2D>();
}

void IBLMaterial::Upload(Shader& shader)
{
	shader.use();
	shader.setInt("irradianceMap", 10);
	shader.setInt("prefilterMap", 11);
	shader.setInt("brdfLUT", 12);
}

void IBLMaterial::Bind()
{
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_CUBE_MAP, IrradianceMap->ID);

	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_CUBE_MAP, PrefilterMap->ID);

	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, BRDFLUT->ID);
}


