#pragma once
#include "Light.h"
class PointLight :
    public Light
{
public:
    PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
    void Upload(Shader& shader, const std::string& name) override;
private:
    float Constant;
    float Linear;
    float Quadratic;
};

