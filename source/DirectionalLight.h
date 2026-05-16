#pragma once
#include "Light.h"
class DirectionalLight :
    public Light
{
public:
    DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    void Upload(Shader& shader, const std::string& name) override;
private:
    glm::vec3 Direction;

};

