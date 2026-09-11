#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer/Framebuffer.h"


class ShadowMap2D
{
public:
	ShadowMap2D(uint32_t width = 1024, uint32_t height = 1024);

	GLuint GetDepthMap() const { return Target->GetDepthMap(); }
	Framebuffer* GetFramebuffer() const { return Target.get(); }

	uint32_t Width = 1024;
	uint32_t Height = 1024;

	glm::mat4 LightSpaceMatrix;

private:
	std::unique_ptr<Framebuffer> Target;
};

