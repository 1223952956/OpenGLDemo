#pragma once
#include <glm/gtc/matrix_transform.hpp>

// TODO
// Release GPU resource
class ShadowMap
{
public:
	unsigned int DepthMapFBO = 0;

	// TODO
	// Use Texture2D
	unsigned int DepthMap = 0;

	unsigned int Width = 1024;
	unsigned int Height = 1024;

	glm::mat4 LightSpaceMatrix;
};

