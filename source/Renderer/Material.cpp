#include "Material.h"

#include "TextureManager.h"

void Material::Bind(Shader& shader)
{
	int slot = 0;

	// base color
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 
		BaseColorTexture? BaseColorTexture->Id : TextureManager::GetWhiteTexture());
	shader.setInt("texture_base_color", slot);
	shader.setVec4("base_color_factor", BaseColorFactor);
	slot++;

	// normal
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 
		NormalTexture ? NormalTexture->Id : TextureManager::GetNormalTexture());
	shader.setInt("texture_normal", slot);
	shader.setFloat("normal_scale", NormalScale);
	slot++;

	// metallic and roughness
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 
		MetallicRoughnessTexture ? MetallicRoughnessTexture->Id : TextureManager::GetWhiteTexture());
	shader.setInt("texture_metallic_roughness", slot);
	shader.setFloat("metallic_factor", MetallicFactor);
	shader.setFloat("roughness_factor", RoughnessFactor);
	slot++;

	// emissive
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 
		EmissiveTexture ? EmissiveTexture->Id : TextureManager::GetBlackTexture());
	shader.setInt("texture_emissive", slot);
	shader.setVec3("emissive_factor", EmissiveFactor);
	slot++;

	// occlusion
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 
		OcclusionTexture ? OcclusionTexture->Id : TextureManager::GetWhiteTexture());
	shader.setInt("texture_occlusion", slot);
	shader.setFloat("occlusion_strength", OcclusionStrength);
	slot++;

	// opaque
	shader.setBool("is_opaque", IsOpaque);
	shader.setFloat("alpha_cutoff", AlphaCutoff);
}
