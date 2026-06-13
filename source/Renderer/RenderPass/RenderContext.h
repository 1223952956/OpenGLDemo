#pragma once

#include "Camera.h"
#include "Scene.h"
#include "Renderer/Framebuffer.h"


class RenderContext
{
public:
	uint32_t ScreenWidth;
	uint32_t ScreenHeight;

	Scene* Scene;

	std::shared_ptr<Framebuffer> SceneFramebuffer;
	std::shared_ptr<Texture2D> BlurColor;
};

