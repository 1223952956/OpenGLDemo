#pragma once
#include <memory>

#include <glm/gtc/matrix_transform.hpp>

#include "IBLMaterial.h"
#include "Shader.h"


// For now, makes it static
class IBLBaker
{
public:
    IBLBaker();
    std::unique_ptr<IBLMaterial> Bake(const std::string& path);

private:
    Shader EquirectToCubemapShader;
    Shader IrradianceShader;
    Shader PrefilterShader;
    Shader BRDFLUTShader;

    GLuint CreateEnvCubemap(GLuint hdrTex);
    GLuint CreateIrradianceMap(GLuint envMap);
    GLuint CreatePrefilterMap(GLuint envMap);
    GLuint CreateBRDFLUT();

    const glm::mat4 CaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    const glm::mat4 CaptureViews[6] =
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };
};

