#pragma once

#include "Camera.h"
#include "Scene.h"
#include "Renderer/Framebuffer.h"


class RenderContext
{
public:
	uint32_t ScreenWidth;
	uint32_t ScreenHeight;

	float CurrTime;
	float DeltaTime;

	Scene* Scene;

	Framebuffer* SceneFramebuffer;
	Texture2D* BlurColor;
};

