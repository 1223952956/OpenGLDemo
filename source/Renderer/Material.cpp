#include "Material.h"

void Material::Bind(Shader& shader)
{
	int slot = 0;

	if (BaseColorTexture)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, BaseColorTexture->Id);

		shader.setInt("material.base_color_texture", slot);

		slot++;
	}
	// TODO:
	// Other Texture
}
