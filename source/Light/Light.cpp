#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 color, float intensity)
	: Position(position)
	, Color(color)
	, Intensity(intensity)
{
}

void Light::Upload(Shader& shader, const std::string& name)
{
	shader.setVec3(name + ".color", Color);
	shader.setFloat(name + ".intensity", Intensity);
}

void Light::SetPosition(glm::vec3 newPos)
{
	Position = newPos;
}
