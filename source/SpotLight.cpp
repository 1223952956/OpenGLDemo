#include "SpotLight.h"

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float cutOff, float outerCutOff)
	:Light(position, ambient, diffuse, specular)
	,Direction(direction)
	,CutOff(cutOff)
	,OuterCufOff(outerCutOff)
{
}

void SpotLight::Upload(Shader& shader, const std::string& name)
{
	Light::Upload(shader, name);
	shader.setVec3(name + ".position", Position);
	shader.setVec3(name + ".direction", Direction);
	shader.setFloat(name + ".cutOff", CutOff);
	shader.setFloat(name + ".outerCutOff", OuterCufOff);
}

void SpotLight::SetDirection(glm::vec3 newDir)
{
	Direction = newDir;
}
