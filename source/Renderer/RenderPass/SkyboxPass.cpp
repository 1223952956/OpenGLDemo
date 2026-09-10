#include "SkyboxPass.h"

#include <glm/gtc/type_ptr.hpp>

#include "Renderer/RenderPrimitives.h"

SkyboxPass::SkyboxPass(const std::string& name, ShaderManager& shaderManager)
	:RenderPass(name)
{
	SkyboxShader = shaderManager.Get("SkyboxShader");
}

void SkyboxPass::Init(RenderContext& context)
{
	SkyboxShader->use();
	SkyboxShader->setInt("environmentMap", 0);
	SkyboxShader->setFloat("intensity", 1.f);
}

void SkyboxPass::Resize(unsigned int width, unsigned int height)
{
}

void SkyboxPass::Execute(RenderContext& context)
{
	context.SceneFramebuffer->Bind();
	SkyboxShader->use();

	UploadCamera(context);

	DrawSkybox(context);
	context.SceneFramebuffer->UnBind();
}

void SkyboxPass::UploadCamera(RenderContext& context)
{
	glm::mat4 projection = glm::perspective(context.Scene->MainCamera->GetFoV(),
		static_cast<float>(context.ScreenWidth) / static_cast<float>(context.ScreenHeight),
		0.1f, 100.0f);
	SkyboxShader->setMat4("projection", 1, GL_FALSE, glm::value_ptr(projection));
}

void SkyboxPass::DrawSkybox(RenderContext& context)
{
	glDepthFunc(GL_LEQUAL);

	glm::mat4 view = context.Scene->MainCamera->GetViewMatrix();

	SkyboxShader->setMat4("view", 1, GL_FALSE, glm::value_ptr(view));
	glActiveTexture(GL_TEXTURE0);
	context.Scene->Enviroment->EnvCubeMap->Bind();
	RenderPrimitives::RenderCube();

	glDepthFunc(GL_LESS);
}
