#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Renderer/Shader.h"


class Light
{
protected:
	glm::vec3 Position;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
public:
	Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	virtual void Upload(Shader& shader, const std::string& name);
	void SetPosition(glm::vec3 newPos);
};

