#pragma once
#include "Light.h"
class DirectionalLight :
    public Light
{
public:
    DirectionalLight(glm::vec3 direction, glm::vec3 color, float intensity);
    void Upload(Shader& shader, const std::string& name) override;
    glm::vec3 const GetDirection() { return Direction; };

private:
    glm::vec3 Direction;

};

