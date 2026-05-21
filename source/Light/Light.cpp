#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 color)
	: Position(position)
	, Color(color)
{
}

void Light::Upload(Shader& shader, const std::string& name)
{
	shader.setVec3(name + ".color", Color);

	//!!!! TEMP !!!!
	shader.setFloat(name + ".range", 100.f);
}

void Light::SetPosition(glm::vec3 newPos)
{
	Position = newPos;
}
