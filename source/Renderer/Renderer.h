#pragma once
#include "Scene.h"
#include "Renderer/IBLBaker.h"
#include "Renderer/ShadowMapBaker.h"
#include "ShaderManager.h"
#include "Renderer/RenderPass/RenderPass.h"
#include "Renderer/Framebuffer.h"

class Renderer
{
public:
    Renderer(ShaderManager& shaderManager);
    void Init(Scene& scene, uint32_t screenWidth, uint32_t screenHeight);
	void Render(Scene& scene, uint32_t screenWidth, uint32_t screenHeight);

private:
    std::vector<std::unique_ptr<RenderPass>> RenderPasses;

    std::shared_ptr<Framebuffer> SceneFramebuffer;

    void InitColorBuffer(uint32_t screenWidth, uint32_t screenHeight);

    // DEBUG
    void DrawDebugQuad(Scene& scene);
    std::shared_ptr<Shader> DebugQuadShader;
};

