#include "DirectionalLight.h"
#include "Math/GlmInterop.h"

DirectionalLight::DirectionalLight(Math::Vec3 direction, Math::Vec3 color, float intensity)
	:Light(Math::Vec3(0.f), color, intensity)
	,Direction(direction)
{
}

void DirectionalLight::Upload(Shader* shader, const std::string& name)
{
	Light::Upload(shader, name);
	shader->setVec3(name + ".direction", Math::ToGlm(Direction));
	shader->setInt(name + ".type", 1);
}