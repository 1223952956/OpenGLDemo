#include "PointLight.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic)
	: Light(position, ambient, diffuse, specular)
	, Constant(constant)
	, Linear(linear)
	, Quadratic(quadratic)
{
}

void PointLight::Upload(Shader& shader, const std::string& name)
{
	Light::Upload(shader, name);
	shader.setVec3(name + ".position", Position);
	shader.setFloat(name + ".constant", Constant);
	shader.setFloat(name + ".linear", Linear);
	shader.setFloat(name + ".quadratic", Quadratic);
}
