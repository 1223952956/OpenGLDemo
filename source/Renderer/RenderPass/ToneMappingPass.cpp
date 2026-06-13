#include "ToneMappingPass.h"
#include "Renderer/RenderPrimitives.h"

ToneMappingPass::ToneMappingPass(const std::string& name, ShaderManager& shaderManager)
	:RenderPass(name)
{
	ToneMappingShader = shaderManager.Get("ToneMappingShader");
}

void ToneMappingPass::Init(RenderContext& context)
{
	ToneMappingShader->use();
	ToneMappingShader->setInt("scene", 0);
	ToneMappingShader->setInt("bloomBlur", 1);
}

void ToneMappingPass::Resize(unsigned int width, unsigned int height)
{
}

void ToneMappingPass::Execute(RenderContext& context)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ToneMappingShader->use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, context.SceneFramebuffer->ColorAttachments[0]->ID);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, context.BlurColor->ID);

	RenderPrimitives::RenderQuad();
}
