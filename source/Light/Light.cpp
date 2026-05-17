#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	: Position(position)
	, Ambient(ambient)
	, Diffuse(diffuse)
	, Specular(specular)
{
}

void Light::Upload(Shader& shader, const std::string& name)
{
	shader.setVec3(name + ".ambient", Ambient);
	shader.setVec3(name + ".diffuse", Diffuse);
	shader.setVec3(name + ".specular", Specular);
}

void Light::SetPosition(glm::vec3 newPos)
{
	Position = newPos;
}
