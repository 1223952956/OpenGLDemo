#include "Renderer.h"

#include <glm/gtc/type_ptr.hpp>

#include "RenderPrimitives.h"
#include "RenderPass/ShadowPass.h"
#include "RenderPass/LightingPass.h"
#include "RenderPass/SkyboxPass.h"

Renderer::Renderer(ShaderManager& shaderManager)
{
	DebugQuadShader = shaderManager.Get("DebugQuadShader");

	RenderPasses.emplace_back(std::make_unique<ShadowPass>("ShadowPass", shaderManager));
	RenderPasses.emplace_back(std::make_unique<LightingPass>("LightingPass", shaderManager));
	RenderPasses.emplace_back(std::make_unique<SkyboxPass>("SkyboxPass", shaderManager));
}

void Renderer::Init(Scene& scene)
{
	DebugQuadShader->use();
	DebugQuadShader->setInt("depthMap", 0);

	RenderContext context;
	context.Scene = &scene;

	for (auto& pass : RenderPasses)
	{
		pass->Init(context);
	}
}

void Renderer::Render(Scene& scene, float screenWidth, float screenHeight)
{
	glClearColor(0.45f, 0.55f, 0.60f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderContext context;
	context.Scene = &scene;
	context.ScreenWidth = screenWidth;
	context.ScreenHeight = screenHeight;

	for (auto& pass : RenderPasses)
	{
		pass->Execute(context);
	}

	// DrawDebugQuad(scene);
}


void Renderer::DrawDebugQuad(Scene& scene)
{
	DebugQuadShader->use();
	DebugQuadShader->setFloat("near_plane", 1.0f);
	DebugQuadShader->setFloat("far_plane", 7.5f);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, scene.DirectionalLights[0].Shadow.DepthMap);
	RenderPrimitives::RenderQuad();
}
