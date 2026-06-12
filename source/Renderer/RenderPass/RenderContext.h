#pragma once
#include "Camera.h"
#include "Scene.h"


class RenderContext
{
public:
	Scene* Scene;

	float ScreenWidth; 
	float ScreenHeight;
};

