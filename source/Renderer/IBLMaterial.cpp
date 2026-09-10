#include "IBLMaterial.h"

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
	IrradianceMap->Bind();
	glActiveTexture(GL_TEXTURE11);
	PrefilterMap->Bind();
	glActiveTexture(GL_TEXTURE12);
	BRDFLUT->Bind();
}


