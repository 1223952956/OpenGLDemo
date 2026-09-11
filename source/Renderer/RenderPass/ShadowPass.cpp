#include "ShadowPass.h"

#include <glm/gtc/type_ptr.hpp>

ShadowPass::ShadowPass(const std::string& name, ShaderManager& shaderManager)
	: RenderPass(name)
{
	PBRShader = shaderManager.Get("PBRShader");
	SimpleDepthShader = shaderManager.Get("SimpleDepthShader");
}

void ShadowPass::Init(RenderContext& context)
{
	auto scene = context.Scene;
	for (int i = 0; i < scene->DirectionalLights.size(); ++i)
	{
		DirectionalShadows.emplace_back(std::make_unique<ShadowMap2D>());
	}
}

void ShadowPass::Resize(unsigned int width, unsigned int height)
{
}

void ShadowPass::Execute(RenderContext& context)
{
	Bake(context.Scene);

	PBRShader->use();

	size_t count = context.Scene->DirectionalLights.size();

	for (int i = 0; i < count; ++i)
	{
		DirectionalLight& dirLight = context.Scene->DirectionalLights[i];
		std::string dirLightName = "dirLightSpaceMatrices[" + std::to_string(i) + "]";
		PBRShader->setMat4(dirLightName, 1, GL_FALSE, glm::value_ptr(DirectionalShadows[i]->LightSpaceMatrix));
		glActiveTexture(GL_TEXTURE13 + i);
		glBindTexture(GL_TEXTURE_2D, DirectionalShadows[i]->GetDepthMap());
	}
}

void ShadowPass::Bake(Scene* scene)
{
	SimpleDepthShader->use();
	// Save current viewport size
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	int scrWidth = viewport[2];
	int scrHeight = viewport[3];

	for (int i = 0; i < DirectionalShadows.size(); ++i)
	{
		auto& dirShadow = DirectionalShadows[i];

		glm::mat4 lightProjection, lightView;

		dirShadow->LightSpaceMatrix = ComputeLightSpaceMatrix(-scene->DirectionalLights[i].GetDirection());

		SimpleDepthShader->setMat4("lightSpaceMatrix", 1, GL_FALSE,
			glm::value_ptr(dirShadow->LightSpaceMatrix));

		// render scene from light's point of view
		glViewport(0, 0, dirShadow->Width, dirShadow->Height);
		dirShadow->GetFramebuffer()->Bind();
		glClear(GL_DEPTH_BUFFER_BIT);

		DrawPieces(scene);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// reset viewport
	glViewport(0, 0, scrWidth, scrHeight);
}

glm::mat4 ShadowPass::ComputeLightSpaceMatrix(glm::vec3 lightPos)
{
	float near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, near_plane, far_plane);

	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	return lightSpaceMatrix;
}

void ShadowPass::DrawPieces(Scene* scene)
{
	SimpleDepthShader->use();

	for (int i = 0; i < scene->Pieces.size(); ++i)
	{
		SimpleDepthShader->setMat4("model", 1, GL_FALSE, glm::value_ptr(scene->Pieces[i]->Transform));
		scene->Pieces[i]->ModelPtr->Draw(SimpleDepthShader.get());
	}
}
