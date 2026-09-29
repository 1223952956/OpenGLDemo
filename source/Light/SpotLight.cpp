#include "SpotLight.h"
#include "Math/GlmInterop.h"

SpotLight::SpotLight(Math::Vec3 position, Math::Vec3 direction, Math::Vec3 color, float cutOff, float outerCutOff, float range, float intensity)
	:Light(position, color, intensity)
	,Direction(direction)
	,InnerCos(cutOff)
	,OuterCos(outerCutOff)
	,Range(range)
{
}

void SpotLight::Upload(Shader* shader, const std::string& name)
{
	Light::Upload(shader, name);
	shader->setVec3(name + ".position", Math::ToGlm(Position));
	shader->setVec3(name + ".direction", Math::ToGlm(Direction));
	shader->setFloat(name + ".innerCos", InnerCos);
	shader->setFloat(name + ".outerCos", OuterCos);

	shader->setInt(name + ".type", 2);
	shader->setFloat(name + ".range", Range);
}

void SpotLight::SetDirection(Math::Vec3 newDir)
{
	Direction = newDir;
}
