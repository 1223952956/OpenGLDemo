#include "GeometryPass.h"

GeometryPass::GeometryPass(const std::string& name, ShaderManager* shaderManager)
	:RenderPass(name)
{
	PBRShader = shaderManager->Get("PBRShader");
}

void GeometryPass::Init(RenderContext& context)
{
}

void GeometryPass::Resize(unsigned int width, unsigned int height)
{
}

void GeometryPass::Execute(RenderContext& context)
{
}
