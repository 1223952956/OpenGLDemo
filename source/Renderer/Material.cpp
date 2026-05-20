#include "Material.h"

void Material::Bind(Shader& shader)
{
	int slot = 0;

	if (BaseColorTexture)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, BaseColorTexture->Id);

		shader.setInt("material.texture_base_color", slot);

		slot++;
	}
	if (EmissiveTexture)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, EmissiveTexture->Id);

		shader.setInt("material.texture_emissive", slot);

		slot++;
	}
	// TODO:
	// Other Texture
}
