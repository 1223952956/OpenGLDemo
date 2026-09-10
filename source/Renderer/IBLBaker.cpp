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
    Texture2D* hdrTex = TextureManager::LoadEquirectangularMap(path);

    auto iblMat = std::make_unique<IBLMaterial>();

	iblMat->EnvCubeMap = std::make_unique<Cubemap>();
	iblMat->IrradianceMap = std::make_unique<Cubemap>();
	iblMat->PrefilterMap = std::make_unique<Cubemap>();
	iblMat->BRDFLUT = std::make_unique<Texture2D>();

    iblMat->EnvCubeMap->Bind();
    UploadEnvCubemap(hdrTex, iblMat->EnvCubeMap.get());
    iblMat->EnvCubeMap->DebugName = hdrTex->DebugName + "::" + "EnvCubeMap";

	iblMat->IrradianceMap->Bind();
	UploadIrradianceMap(iblMat->EnvCubeMap.get(), iblMat->IrradianceMap.get());
	iblMat->IrradianceMap->DebugName = hdrTex->DebugName + "::" + "IrradianceMap";

	iblMat->PrefilterMap->Bind();
	UploadPrefilterMap(iblMat->EnvCubeMap.get(), iblMat->PrefilterMap.get());
	iblMat->PrefilterMap->DebugName = hdrTex->DebugName + "::" + "PrefilterMap";

	iblMat->BRDFLUT->Bind();
	UploadBRDFLUT(iblMat->BRDFLUT.get());
	iblMat->BRDFLUT->DebugName = hdrTex->DebugName + "::" + "BRDFLUT";

    return iblMat;
}

void IBLBaker::UploadEnvCubemap(Texture2D* hdrTex, Cubemap* envMap)
{
    // TODO 
    // delete captureFBO when baking is done
    // Frame/Render Buffer
    unsigned int captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    // Cubemap
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
	hdrTex->Bind();

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
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envMap->GetID(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderPrimitives::RenderCube(); // renders a 1x1 cube
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // configure the viewport to the original framebuffer's screen dimensions
    glViewport(0, 0, scrWidth, scrHeight);
}

void IBLBaker::UploadIrradianceMap(Cubemap* envMap, Cubemap* irradianceMap)
{
    unsigned int captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);


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
    envMap->Bind();
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
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap->GetID(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderPrimitives::RenderCube(); // renders a 1x1 cube
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // configure the viewport to the original framebuffer's screen dimensions
    glViewport(0, 0, scrWidth, scrHeight);
}

void IBLBaker::UploadPrefilterMap(Cubemap* envMap, Cubemap* prefilterMap)
{
    unsigned int captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

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
	envMap->Bind();

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
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap->GetID(), mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            RenderPrimitives::RenderCube();
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // configure the viewport to the original framebuffer's screen dimensions
    glViewport(0, 0, scrWidth, scrHeight);
}

void IBLBaker::UploadBRDFLUT(Texture2D* brdfLUT)
{
    // pre-allocate enough memory for the LUT texture.
    brdfLUT->Bind();
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
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUT->GetID(), 0);

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
}