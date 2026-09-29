#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Renderer/Shader.h"
#include "Math/Vector.h"


class Light
{
protected:
	Math::Vec3 Position;
	Math::Vec3 Color;
	float Intensity;
public:
	Light(Math::Vec3 position, Math::Vec3 color, float intensity);
	virtual void Upload(Shader* shader, const std::string& name);

	void SetPosition(Math::Vec3 newPos);
	Math::Vec3 GetPosition() const { return Position; }
	Math::Vec3 GetColor() const { return Color; }
	float GetIntensity() const { return Intensity; }
};

