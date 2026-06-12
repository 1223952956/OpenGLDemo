#pragma once
#include "Light.h"
class SpotLight :
    public Light
{
public:
    SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float cutOff, float outerCutOff, float range, float intensity);
    void Upload(Shader* shader, const std::string& name) override;
    void SetDirection(glm::vec3 newDir);

private:
    glm::vec3 Direction;
    float InnerCos;
    float OuterCos;
    float Range;
};

