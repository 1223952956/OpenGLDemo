#pragma once
#include <glm/gtc/matrix_transform.hpp>

class ShadowMap
{
public:
	unsigned int DepthMapFBO = 0;

	unsigned int DepthMap = 0;

	unsigned int Width = 1024;
	unsigned int Height = 1024;

	glm::mat4 LightSpaceMatrix;
};

