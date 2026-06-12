#include "ShadowPass.h"

#include <glm/gtc/type_ptr.hpp>

ShadowPass::ShadowPass(const std::string& name, ShaderManager& shaderManager)
	: RenderPass(name),
	ShadowBaker(shaderManager)
{
	PBRShader = shaderManager.Get("PBRShader");
}

void ShadowPass::Init(RenderContext& context)
{
	ShadowBaker.Init(context.Scene);
}

void ShadowPass::Resize(unsigned int width, unsigned int height)
{
}

void ShadowPass::Execute(RenderContext& context)
{
	ShadowBaker.Bake(context.Scene);

	PBRShader->use();

	for (int i = 0; i < context.Scene->DirectionalLights.size(); ++i)
	{
		DirectionalLight& dirLight = context.Scene->DirectionalLights[i];
		std::string dirLightName = "dirLightSpaceMatrices[" + std::to_string(i) + "]";
		PBRShader->setMat4(dirLightName, 1, GL_FALSE, glm::value_ptr(dirLight.Shadow.LightSpaceMatrix));
		glActiveTexture(GL_TEXTURE13 + i);
		glBindTexture(GL_TEXTURE_2D, dirLight.Shadow.DepthMap);
	}
}
