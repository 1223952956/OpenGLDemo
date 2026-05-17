#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	:Light(glm::vec3(0.f), ambient, diffuse, specular)
	,Direction(direction)
{
}

void DirectionalLight::Upload(Shader& shader, const std::string& name)
{
	Light::Upload(shader, name);
	shader.setVec3(name + ".direction", Direction);
}