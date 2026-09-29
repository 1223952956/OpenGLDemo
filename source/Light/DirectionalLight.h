#pragma once
#include "Light.h"


class DirectionalLight :
    public Light
{
public:
    DirectionalLight(Math::Vec3 direction, Math::Vec3 color, float intensity);
    void Upload(Shader* shader, const std::string& name) override;
    Math::Vec3 GetDirection() const { return Direction; };

private:
    Math::Vec3 Direction;
};

