#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Renderer/Shader.h"


class Light
{
protected:
	glm::vec3 Position;
	glm::vec3 Color;
public:
	Light(glm::vec3 position, glm::vec3 color);
	virtual void Upload(Shader& shader, const std::string& name);
	void SetPosition(glm::vec3 newPos);
};

