#include "PointLight.h"
#include "Math/GlmInterop.h"

PointLight::PointLight(Math::Vec3 position, Math::Vec3 color, float range, float intensity)
	: Light(position, color, intensity)
	, Range(range)
{
}

void PointLight::Upload(Shader* shader, const std::string& name)
{
	Light::Upload(shader, name);
	shader->setVec3(name + ".position", Math::ToGlm(Position));
	shader->setInt(name + ".type", 0);
	shader->setFloat(name + ".range", Range);
}
