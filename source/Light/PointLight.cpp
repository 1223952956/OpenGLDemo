#include "PointLight.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic, float range)
	: Light(position, color)
	, Constant(constant)
	, Linear(linear)
	, Quadratic(quadratic)
	, Range(range)
{
}

void PointLight::Upload(Shader& shader, const std::string& name)
{
	Light::Upload(shader, name);
	shader.setVec3(name + ".position", Position);
	shader.setInt(name + ".type", 0);
	shader.setFloat(name + ".constant", Constant);
	shader.setFloat(name + ".linear", Linear);
	shader.setFloat(name + ".quadratic", Quadratic);
	shader.setFloat(name + ".range", Range);
}
