#pragma once
#include <memory>

#include <glm/gtc/matrix_transform.hpp>

#include "IBLMaterial.h"
#include "Shader.h"


// For now, makes it static
class IBLBaker
{
public:
    static std::unique_ptr<IBLMaterial> Bake(const std::string& path, Shader& envMapShader, Shader& irradianceMapShader, Shader& prefilterMapShader);

    static void renderCube();

private:
    static GLuint CreateEnvCubemap(GLuint hdrTex, Shader& shader);
    static GLuint CreateIrradianceMap(GLuint envMap, Shader& shader);
    static GLuint CreatePrefilterMap(GLuint envMap);
    static GLuint CreateBRDFLUT();

    inline static const glm::mat4 CaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	inline static const glm::mat4 CaptureViews[6] =
	{
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};;
};

