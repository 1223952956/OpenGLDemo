#pragma once
#include "Light.h"
class PointLight :
    public Light
{
public:
    PointLight(glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic);
    void Upload(Shader& shader, const std::string& name) override;
private:
    float Constant;
    float Linear;
    float Quadratic;
};

