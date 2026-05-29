#pragma once
#include "Scene.h"

class Renderer
{
public:
    Renderer();
	void Render(Scene& scene, float screenWidth, float screenHeight);

    Shader PBRShader;
    Shader ShadowShader;
    Shader SkyboxShader;
private:
    void UploadCamera(Scene& scene, float screenWidth, float screenHeight);
    void UploadLights(Scene& scene);

    void DrawPieces(Scene& scene);
    void DrawSkyBox(Scene& scene);
};

