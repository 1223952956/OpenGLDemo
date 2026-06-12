#pragma once
#include "Scene.h"
#include "Renderer/IBLBaker.h"
#include "Renderer/ShadowMapBaker.h"
#include "ShaderManager.h"
#include "Renderer/RenderPass/RenderPass.h"

class Renderer
{
public:
    Renderer(ShaderManager& shaderManager);
    void Init(Scene& scene);
	void Render(Scene& scene, float screenWidth, float screenHeight);

private:
    std::vector<std::unique_ptr<RenderPass>> RenderPasses;


    void DrawDebugQuad(Scene& scene);

    std::shared_ptr<Shader> DebugQuadShader;
};

