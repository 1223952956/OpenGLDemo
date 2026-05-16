#pragma once
#include "Light.h"
class SpotLight :
    public Light
{
public:
    SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float cutOff, float outerCutOff);
    void Upload(Shader& shader, const std::string& name) override;
    void SetDirection(glm::vec3 newDir);

private:
    glm::vec3 Direction;
    float CutOff;
    float OuterCufOff;
};

