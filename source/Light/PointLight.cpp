#include "PointLight.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 color, float range, float intensity)
	: Light(position, color, intensity)
	, Range(range)
{
}

void PointLight::Upload(Shader* shader, const std::string& name)
{
	Light::Upload(shader, name);
	shader->setVec3(name + ".position", Position);
	shader->setInt(name + ".type", 0);
	shader->setFloat(name + ".range", Range);
}
