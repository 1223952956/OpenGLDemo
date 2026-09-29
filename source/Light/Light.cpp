#include "Light.h"
#include "Math/GlmInterop.h"

Light::Light(Math::Vec3 position, Math::Vec3 color, float intensity)
	: Position(position)
	, Color(color)
	, Intensity(intensity)
{
}

void Light::Upload(Shader* shader, const std::string& name)
{
	shader->setVec3(name + ".color", Math::ToGlm(Color));
	shader->setFloat(name + ".intensity", Intensity);
}

void Light::SetPosition(Math::Vec3 newPos)
{
	Position = newPos;
}
