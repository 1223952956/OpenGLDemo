#pragma once
#include "Light.h"
class SpotLight :
    public Light
{
public:
    SpotLight(Math::Vec3 position, Math::Vec3 direction, Math::Vec3 color, float cutOff, float outerCutOff, float range, float intensity);
    void Upload(Shader* shader, const std::string& name) override;

    void SetDirection(Math::Vec3 newDir);
	Math::Vec3 GetDirection() const { return Direction; }
	float GetInnerCos() const { return InnerCos; }
	float GetOuterCos() const { return OuterCos; }
	float GetRange() const { return Range; }

private:
    Math::Vec3 Direction;
    float InnerCos;
    float OuterCos;
    float Range;
};

