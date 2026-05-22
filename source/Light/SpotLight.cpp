#include "SpotLight.h"

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float cutOff, float outerCutOff, float range)
	:Light(position, color)
	,Direction(direction)
	,InnerCos(cutOff)
	,OuterCos(outerCutOff)
	,Range(range)
{
}

void SpotLight::Upload(Shader& shader, const std::string& name)
{
	Light::Upload(shader, name);
	shader.setVec3(name + ".position", Position);
	shader.setVec3(name + ".direction", Direction);
	shader.setFloat(name + ".innerCos", InnerCos);
	shader.setFloat(name + ".outerCos", OuterCos);

	shader.setInt(name + ".type", 2);
	shader.setFloat(name + ".range", Range);
}

void SpotLight::SetDirection(glm::vec3 newDir)
{
	Direction = newDir;
}
