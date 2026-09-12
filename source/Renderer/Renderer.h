#pragma once
#include "Scene.h"
#include "Renderer/IBLBaker.h"
#include "Renderer/RenderPass/RenderPass.h"
#include "Renderer/Framebuffer.h"

class ShaderManager;

class Renderer
{
public:
    Renderer();
    ~Renderer();
    void Initialize(Scene* scene, uint32_t screenWidth, uint32_t screenHeight);
	void Render(Scene* scene, uint32_t screenWidth, uint32_t screenHeight, float currTime, float deltaTime);
	void Resize(uint32_t screenWidth, uint32_t screenHeight);
private:
    std::unique_ptr<ShaderManager> CShaderManager;

    std::vector<std::unique_ptr<RenderPass>> RenderPasses;

    std::unique_ptr<Framebuffer> SceneFramebuffer;

    void InitColorBuffer(uint32_t screenWidth, uint32_t screenHeight);

    // DEBUG
    void DrawDebugQuad(Scene& scene);
    std::shared_ptr<Shader> DebugQuadShader;
};

