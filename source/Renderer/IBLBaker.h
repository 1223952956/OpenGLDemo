#pragma once
#include <memory>

#include <glm/gtc/matrix_transform.hpp>

#include "IBLMaterial.h"
#include "Shader.h"
#include "ShaderManager.h"


// For now, makes it static
class IBLBaker
{
public:
    IBLBaker(ShaderManager* shaderManager);
    void Init();
    std::unique_ptr<IBLMaterial> Bake(const std::string& path);

private:
    std::shared_ptr<Shader> EquirectToCubemapShader;
    std::shared_ptr<Shader> IrradianceShader;
    std::shared_ptr<Shader> PrefilterShader;
    std::shared_ptr<Shader> BRDFLUTShader;

    void UploadEnvCubemap(Texture2D* hdrTex, Cubemap* envMap);
    void UploadIrradianceMap(Cubemap* envMap, Cubemap* irradianceMap);
    void UploadPrefilterMap(Cubemap* envMap, Cubemap* prefilterMap);
    void UploadBRDFLUT(Texture2D* brdfLUT);

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

