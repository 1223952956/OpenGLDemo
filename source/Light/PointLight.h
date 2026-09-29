#pragma once
#include "Light.h"


class PointLight :
    public Light
{
public:
    PointLight(Math::Vec3 position, Math::Vec3 color, float range, float intensity);
    void Upload(Shader* shader, const std::string& name) override;
	float GetRange() const { return Range; }
private:
    float Range;
};

