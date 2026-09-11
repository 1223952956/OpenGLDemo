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
	context.SceneFramebuffer = SceneFramebuffer.get();

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
	context.SceneFramebuffer = SceneFramebuffer.get();

	for (auto& pass : RenderPasses)
	{
		pass->Execute(context);
	}

	// DrawDebugQuad(scene);
}


void Renderer::InitColorBuffer(uint32_t screenWidth, uint32_t screenHeight)
{
	FramebufferSpecification frameBufferSpec;
	frameBufferSpec.Width = screenWidth;
	frameBufferSpec.Height = screenHeight;
	frameBufferSpec.Name = "SceneFramebuffer";
	frameBufferSpec.DepthAttachment.Type = DepthAttachmentType::Renderbuffer;
	frameBufferSpec.DepthAttachment.RenderbufferFormat = GL_DEPTH_COMPONENT;

	for (size_t i = 0; i < 2; ++i)
	{
		frameBufferSpec.ColorAttachments.emplace_back();
		auto& colorAttachment = frameBufferSpec.ColorAttachments.back();

		colorAttachment.Width = frameBufferSpec.Width;
		colorAttachment.Height = frameBufferSpec.Height;
		colorAttachment.InternalFormat = GL_RGB16F;
		colorAttachment.DataFormat = GL_RGB;
		colorAttachment.DataType = GL_FLOAT;
		colorAttachment.SamplerSpec.WrapS = GL_CLAMP_TO_EDGE;
		colorAttachment.SamplerSpec.WrapT = GL_CLAMP_TO_EDGE;
		colorAttachment.SamplerSpec.MinFilter = GL_LINEAR;
		colorAttachment.SamplerSpec.MagFilter = GL_LINEAR;
		colorAttachment.GenerateMipmaps = false;
	}
	frameBufferSpec.ColorAttachments[0].DebugName = "Renderer::SceneColor";
	frameBufferSpec.ColorAttachments[1].DebugName = "Renderer::BrightColor";

	SceneFramebuffer = std::make_unique<Framebuffer>(frameBufferSpec);
}

void Renderer::DrawDebugQuad(Scene& scene)
{
	//DebugQuadShader->use();
	//DebugQuadShader->setFloat("near_plane", 1.0f);
	//DebugQuadShader->setFloat("far_plane", 7.5f);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, scene.DirectionalLights[0].Shadow.DepthMap);
	//RenderPrimitives::RenderQuad();
}
