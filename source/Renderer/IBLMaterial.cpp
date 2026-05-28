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
	glBindTexture(GL_TEXTURE_CUBE_MAP, IrradianceMap.Id);

	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_CUBE_MAP, PrefilterMap.Id);

	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, BRDFLUT.Id);
}


