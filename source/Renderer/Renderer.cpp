#include "Renderer.h"

#include <glm/gtc/type_ptr.hpp>

#include "RenderPrimitives.h"
#include "RenderPass/ShadowPass.h"
#include "RenderPass/LightingPass.h"
#include "RenderPass/SkyboxPass.h"
#include "RenderPass/BloomPass.h"
#include "RenderPass/ToneMappingPass.h"

Renderer::Renderer(ShaderManager& shaderManager)
{
	DebugQuadShader = shaderManager.Get("DebugQuadShader");

	RenderPasses.emplace_back(std::make_unique<ShadowPass>("ShadowPass", shaderManager));
	RenderPasses.emplace_back(std::make_unique<LightingPass>("LightingPass", shaderManager));
	RenderPasses.emplace_back(std::make_unique<BloomPass>("BloomPass", shaderManager));
	RenderPasses.emplace_back(std::make_unique<SkyboxPass>("SkyboxPass", shaderManager));
	RenderPasses.emplace_back(std::make_unique<ToneMappingPass>("ToneMappingPass", shaderManager));
}

void Renderer::Init(Scene& scene, uint32_t screenWidth, uint32_t screenHeight)
{
	DebugQuadShader->use();
	DebugQuadShader->setInt("depthMap", 0);

	InitColorBuffer(screenWidth, screenHeight);

	RenderContext context;
	context.Scene = &scene;
	context.ScreenWidth = screenWidth;
	context.ScreenHeight = screenHeight;
	context.SceneFramebuffer = SceneFramebuffer;

	for (auto& pass : RenderPasses)
	{
		pass->Init(context);
	}
}

void Renderer::Render(Scene& scene, uint32_t screenWidth, uint32_t screenHeight, float currTime, float deltaTime)
{
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderContext context;
	context.Scene = &scene;
	context.CurrTime = currTime;
	context.DeltaTime = deltaTime;
	context.ScreenWidth = screenWidth;
	context.ScreenHeight = screenHeight;
	context.SceneFramebuffer = SceneFramebuffer;

	for (auto& pass : RenderPasses)
	{
		pass->Execute(context);
	}

	// DrawDebugQuad(scene);
}


void Renderer::InitColorBuffer(uint32_t screenWidth, uint32_t screenHeight)
{
	// Set up floating point framebuffer to render scene to
	GLuint sceneFBO;
	glGenFramebuffers(1, &sceneFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);
	GLuint colorBuffers[2];
	glGenTextures(2, colorBuffers);
	for (GLuint i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB16F, screenWidth, screenHeight, 0, GL_RGB, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0
		);
	}

	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer not complete!" << std::endl;
	}

	auto sceneColor = std::make_shared<Texture2D>("Renderer::SceneColor", colorBuffers[0]);
	auto brightColor = std::make_shared<Texture2D>("Renderer::BrightColor", colorBuffers[1]);

	SceneFramebuffer = std::make_shared<Framebuffer>("Renderer::SceneFramebuffer", sceneFBO);
	SceneFramebuffer->ColorAttachments = { sceneColor, brightColor};

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
