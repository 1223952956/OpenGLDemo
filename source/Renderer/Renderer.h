#pragma once
#include "Scene.h"
#include "Renderer/ShadowMapBaker.h"

class Renderer
{
public:
    Renderer();
    void Init(Scene& scene);
	void Render(Scene& scene, float screenWidth, float screenHeight);

    Shader PBRShader;
    Shader SkyboxShader;
    Shader DebugQuadShader;
private:
    void UploadCamera(Scene& scene, float screenWidth, float screenHeight);
    void UploadLights(Scene& scene);

    void DrawShadow(Scene& scene);
    void DrawPieces(Scene& scene);
    void DrawSkyBox(Scene& scene);
    void DrawDebugQuad(Scene& scene);

    ShadowMapBaker ShadowBaker;
};

