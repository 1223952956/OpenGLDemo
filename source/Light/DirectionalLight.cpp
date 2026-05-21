#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color)
	:Light(glm::vec3(0.f), color)
	,Direction(direction)
{
}

void DirectionalLight::Upload(Shader& shader, const std::string& name)
{
	Light::Upload(shader, name);
	shader.setVec3(name + ".direction", Direction);
	shader.setInt(name + ".type", 1);
}