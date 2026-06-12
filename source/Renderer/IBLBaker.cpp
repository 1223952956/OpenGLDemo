#include "IBLBaker.h"

#include <glm/gtc/type_ptr.hpp>

#include "TextureManager.h"
#include "RenderPrimitives.h"

IBLBaker::IBLBaker(ShaderManager& shaderManager)
{
    EquirectToCubemapShader = shaderManager.Get("EquirectToCubemapShader");
    IrradianceShader = shaderManager.Get("IrradianceShader");
    PrefilterShader = shaderManager.Get("PrefilterShader");
    BRDFLUTShader = shaderManager.Get("BRDFLUTShader");
}

void IBLBaker::Init()
{
}

std::unique_ptr<IBLMaterial> IBLBaker::Bake(const std::string& path)
{
    Texture2D* hdrTex = TextureManager::Load(path);

    auto iblMat = std::make_unique<IBLMaterial>();

    iblMat->EnvCubeMap.ID = CreateEnvCubemap(hdrTex->ID);
    iblMat->EnvCubeMap.Name = hdrTex->Path + "::" + "EnvCubeMap";

    iblMat->IrradianceMap.ID = CreateIrradianceMap(iblMat->EnvCubeMap.ID);
    iblMat->IrradianceMap.Name = hdrTex->Path + "::" + "IrradianceMap";

    iblMat->PrefilterMap.ID = CreatePrefilterMap(iblMat->EnvCubeMap.ID);
    iblMat->PrefilterMap.Name = hdrTex->Path + "::" + "PrefilterMap";

    iblMat->BRDFLUT.ID = CreateBRDFLUT();
    iblMat->BRDFLUT.Path = hdrTex->Path + "::" + "BRDFLUT";

    return iblMat;
}

GLuint IBLBaker::CreateEnvCubemap(GLuint hdrTex)
{
    // Frame/Render Buffer
    unsigned int captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    // Cubemap
    unsigned int envCubemap;
    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        // note that we store each face with 16 bit floating point values
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
            512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    // convert HDR equirectangular environment map to cubemap equivalent
    EquirectToCubemapShader->use();
    EquirectToCubemapShader->setInt("equirectangularMap", 0);
    EquirectToCubemapShader->setMat4("projection", 1, GL_FALSE, glm::value_ptr(IBLBaker::CaptureProjection));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTex);

    // Save current viewport size
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int scrWidth = viewport[2];
    int scrHeight = viewport[3];

    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    
    // Render cubemap on framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        EquirectToCubemapShader->setMat4("view", 1, GL_FALSE, glm::value_ptr(IBLBaker::CaptureViews[i]));
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderPrimitives::RenderCube(); // renders a 1x1 cube
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // configure the viewport to the original framebuffer's screen dimensions
    glViewport(0, 0, scrWidth, scrHeight);

    return envCubemap;
}

GLuint IBLBaker::CreateIrradianceMap(GLuint envMap)
{
    unsigned int captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    unsigned int irradianceMap;
    glGenTextures(1, &irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0,
            GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    IrradianceShader->use();
    IrradianceShader->setInt("environmentMap", 0);
    IrradianceShader->setMat4("projection", 1, GL_FALSE, glm::value_ptr(IBLBaker::CaptureProjection));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envMap);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // Save current viewport size
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int scrWidth = viewport[2];
    int scrHeight = viewport[3];

    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.

        // Render cubemap on framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        IrradianceShader->setMat4("view", 1, GL_FALSE, glm::value_ptr(IBLBaker::CaptureViews[i]));
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderPrimitives::RenderCube(); // renders a 1x1 cube
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // configure the viewport to the original framebuffer's screen dimensions
    glViewport(0, 0, scrWidth, scrHeight);

    return irradianceMap;
}

GLuint IBLBaker::CreatePrefilterMap(GLuint envMap)
{
    unsigned int captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    unsigned int prefilterMap;
    glGenTextures(1, &prefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    PrefilterShader->use();
    PrefilterShader->setInt("environmentMap", 0);
    PrefilterShader->setMat4("projection", 1, GL_FALSE, glm::value_ptr(IBLBaker::CaptureProjection));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envMap);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    unsigned int maxMipLevels = 5;

    // Save current viewport size
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int scrWidth = viewport[2];
    int scrHeight = viewport[3];

    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        unsigned int mipWidth = 128 * std::pow(0.5, mip);
        unsigned int mipHeight = 128 * std::pow(0.5, mip);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        PrefilterShader->setFloat("roughness", roughness);
        for (unsigned int i = 0; i < 6; ++i)
        {
            PrefilterShader->setMat4("view", 1, GL_FALSE, glm::value_ptr(IBLBaker::CaptureViews[i]));
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            RenderPrimitives::RenderCube();
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // configure the viewport to the original framebuffer's screen dimensions
    glViewport(0, 0, scrWidth, scrHeight);

    return prefilterMap;
}

GLuint IBLBaker::CreateBRDFLUT()
{
    unsigned int brdfLUTTexture;
    glGenTextures(1, &brdfLUTTexture);

    // pre-allocate enough memory for the LUT texture.
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned int captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

    // Save current viewport size
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int scrWidth = viewport[2];
    int scrHeight = viewport[3];

    glViewport(0, 0, 512, 512);
    BRDFLUTShader->use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    RenderPrimitives::RenderQuad();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // configure the viewport to the original framebuffer's screen dimensions
    glViewport(0, 0, scrWidth, scrHeight);

    return brdfLUTTexture;
}