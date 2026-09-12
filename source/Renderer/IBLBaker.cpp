#include "IBLBaker.h"

#include <glm/gtc/type_ptr.hpp>

#include "TextureManager.h"
#include "RenderPrimitives.h"
#include "Renderer/Framebuffer.h"

IBLBaker::IBLBaker(ShaderManager* shaderManager)
{
    EquirectToCubemapShader = shaderManager->Get("EquirectToCubemapShader");
    IrradianceShader = shaderManager->Get("IrradianceShader");
    PrefilterShader = shaderManager->Get("PrefilterShader");
    BRDFLUTShader = shaderManager->Get("BRDFLUTShader");
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
    iblMat->EnvCubeMap->SetDebugName(hdrTex->GetDebugName() + "::" + "EnvCubeMap");

	iblMat->IrradianceMap->Bind();
	UploadIrradianceMap(iblMat->EnvCubeMap.get(), iblMat->IrradianceMap.get());
	iblMat->IrradianceMap->SetDebugName(hdrTex->GetDebugName() + "::" + "IrradianceMap");

	iblMat->PrefilterMap->Bind();
	UploadPrefilterMap(iblMat->EnvCubeMap.get(), iblMat->PrefilterMap.get());
	iblMat->PrefilterMap->SetDebugName(hdrTex->GetDebugName() + "::" + "PrefilterMap");

	iblMat->BRDFLUT->Bind();
	UploadBRDFLUT(iblMat->BRDFLUT.get());
	iblMat->BRDFLUT->SetDebugName(hdrTex->GetDebugName() + "::" + "BRDFLUT");

    return iblMat;
}

void IBLBaker::UploadEnvCubemap(Texture2D* hdrTex, Cubemap* envMap)
{
	// Create framebuffer for capturing the cubemap
	FramebufferSpecification framebufferSpec;
	framebufferSpec.Width = 512;
	framebufferSpec.Height = 512;
	framebufferSpec.DepthAttachment.Type = DepthAttachmentType::Renderbuffer;
	framebufferSpec.DepthAttachment.RenderbufferFormat = GL_DEPTH_COMPONENT24;

	Framebuffer captureFramebuffer(framebufferSpec);


    // Cubemap
	CubemapData cubemapData;
	cubemapData.Width = framebufferSpec.Width;
	cubemapData.Height = framebufferSpec.Height;
    cubemapData.InternalFormat = GL_RGB16F;
	cubemapData.DataFormat = GL_RGB;
	cubemapData.DataType = GL_FLOAT;
	cubemapData.SamplerSpec.WrapS = GL_CLAMP_TO_EDGE;
	cubemapData.SamplerSpec.WrapT = GL_CLAMP_TO_EDGE;
	cubemapData.SamplerSpec.WrapR = GL_CLAMP_TO_EDGE;
	cubemapData.SamplerSpec.MinFilter = GL_LINEAR;
	cubemapData.SamplerSpec.MagFilter = GL_LINEAR;

	envMap->SetData(cubemapData);

    // Save current viewport size
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // configure the viewport to the capture dimensions.
    glViewport(0, 0, framebufferSpec.Width, framebufferSpec.Height);

    // convert HDR equirectangular environment map to cubemap equivalent
    EquirectToCubemapShader->use();
    EquirectToCubemapShader->setInt("equirectangularMap", 0);
    EquirectToCubemapShader->setMat4("projection", 1, GL_FALSE, glm::value_ptr(IBLBaker::CaptureProjection));
    glActiveTexture(GL_TEXTURE0);
    hdrTex->Bind();
	captureFramebuffer.Bind();

    // Render cubemap on framebuffer
    for (unsigned int i = 0; i < 6; ++i)
    {
        EquirectToCubemapShader->setMat4("view", 1, GL_FALSE, glm::value_ptr(IBLBaker::CaptureViews[i]));
		captureFramebuffer.AttachCubemapFace(0, envMap, i);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderPrimitives::RenderCube(); // renders a 1x1 cube
    }

	envMap->GenerateMipmaps();

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // configure the viewport to the original framebuffer's screen dimensions
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void IBLBaker::UploadIrradianceMap(Cubemap* envMap, Cubemap* irradianceMap)
{
    // Create framebuffer for capturing the cubemap
    FramebufferSpecification framebufferSpec;
    framebufferSpec.Width = 32;
    framebufferSpec.Height = 32;
    framebufferSpec.DepthAttachment.Type = DepthAttachmentType::Renderbuffer;
    framebufferSpec.DepthAttachment.RenderbufferFormat = GL_DEPTH_COMPONENT24;

    Framebuffer captureFramebuffer(framebufferSpec);


    // Cubemap
    CubemapData cubemapData;
    cubemapData.Width = framebufferSpec.Width;
    cubemapData.Height = framebufferSpec.Height;
    cubemapData.InternalFormat = GL_RGB16F;
    cubemapData.DataFormat = GL_RGB;
    cubemapData.DataType = GL_FLOAT;
    cubemapData.SamplerSpec.WrapS = GL_CLAMP_TO_EDGE;
    cubemapData.SamplerSpec.WrapT = GL_CLAMP_TO_EDGE;
    cubemapData.SamplerSpec.WrapR = GL_CLAMP_TO_EDGE;
    cubemapData.SamplerSpec.MinFilter = GL_LINEAR;
    cubemapData.SamplerSpec.MagFilter = GL_LINEAR;

    irradianceMap->SetData(cubemapData);

    // Save current viewport size
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // configure the viewport to the capture dimensions.
    glViewport(0, 0, framebufferSpec.Width, framebufferSpec.Height);

    IrradianceShader->use();
    IrradianceShader->setInt("environmentMap", 0);
    IrradianceShader->setMat4("projection", 1, GL_FALSE, glm::value_ptr(IBLBaker::CaptureProjection));
    glActiveTexture(GL_TEXTURE0);
    envMap->Bind();
    captureFramebuffer.Bind();

    // Render cubemap on framebuffer
    for (unsigned int i = 0; i < 6; ++i)
    {
        IrradianceShader->setMat4("view", 1, GL_FALSE, glm::value_ptr(IBLBaker::CaptureViews[i]));
		captureFramebuffer.AttachCubemapFace(0, irradianceMap, i);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderPrimitives::RenderCube(); // renders a 1x1 cube
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // configure the viewport to the original framebuffer's screen dimensions
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void IBLBaker::UploadPrefilterMap(Cubemap* envMap, Cubemap* prefilterMap)
{
    // Create framebuffer for capturing the cubemap
    FramebufferSpecification framebufferSpec;
    framebufferSpec.Width = 128;
    framebufferSpec.Height = 128;
    framebufferSpec.DepthAttachment.Type = DepthAttachmentType::Renderbuffer;
    framebufferSpec.DepthAttachment.RenderbufferFormat = GL_DEPTH_COMPONENT24;

    Framebuffer captureFramebuffer(framebufferSpec);


    // Cubemap
    CubemapData cubemapData;
    cubemapData.Width = framebufferSpec.Width;
    cubemapData.Height = framebufferSpec.Height;
	cubemapData.MipLevels = 5;
    cubemapData.InternalFormat = GL_RGB16F;
    cubemapData.DataFormat = GL_RGB;
    cubemapData.DataType = GL_FLOAT;
    cubemapData.SamplerSpec.WrapS = GL_CLAMP_TO_EDGE;
    cubemapData.SamplerSpec.WrapT = GL_CLAMP_TO_EDGE;
    cubemapData.SamplerSpec.WrapR = GL_CLAMP_TO_EDGE;
    cubemapData.SamplerSpec.MinFilter = GL_LINEAR_MIPMAP_LINEAR;
    cubemapData.SamplerSpec.MagFilter = GL_LINEAR;

    prefilterMap->SetData(cubemapData);

    // Save current viewport size
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    PrefilterShader->use();
    PrefilterShader->setInt("environmentMap", 0);
    PrefilterShader->setMat4("projection", 1, GL_FALSE, glm::value_ptr(IBLBaker::CaptureProjection));
    glActiveTexture(GL_TEXTURE0);
    envMap->Bind();
	captureFramebuffer.Bind();

    for (unsigned int mip = 0; mip < cubemapData.MipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        unsigned int mipSize = 128 * std::pow(0.5, mip);

		captureFramebuffer.ResizeDepthRenderbuffer(mipSize, mipSize);
        glViewport(0, 0, mipSize, mipSize);

        float roughness = (float)mip / (float)(cubemapData.MipLevels - 1);
        PrefilterShader->setFloat("roughness", roughness);
        for (unsigned int i = 0; i < 6; ++i)
        {
            PrefilterShader->setMat4("view", 1, GL_FALSE, glm::value_ptr(IBLBaker::CaptureViews[i]));
			captureFramebuffer.AttachCubemapFace(0, prefilterMap, i, mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            RenderPrimitives::RenderCube();
        }
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // configure the viewport to the original framebuffer's screen dimensions
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void IBLBaker::UploadBRDFLUT(Texture2D* brdfLUT)
{
    // pre-allocate enough memory for the LUT texture.

    Texture2DData dataToSet;
    dataToSet.Width = 512;
    dataToSet.Height = 512;
    dataToSet.InternalFormat = GL_RG16F;
    dataToSet.DataFormat = GL_RG;
    dataToSet.DataType = GL_FLOAT;
    dataToSet.SamplerSpec.WrapS = GL_CLAMP_TO_EDGE;
    dataToSet.SamplerSpec.WrapT = GL_CLAMP_TO_EDGE;
    dataToSet.SamplerSpec.MinFilter = GL_LINEAR;
    dataToSet.SamplerSpec.MagFilter = GL_LINEAR;
    dataToSet.GenerateMipmaps = false;

	brdfLUT->SetData(dataToSet);

    FramebufferSpecification framebufferSpec;
    framebufferSpec.Width = 512;
    framebufferSpec.Height = 512;
    framebufferSpec.DepthAttachment.Type = DepthAttachmentType::Renderbuffer;
    framebufferSpec.DepthAttachment.RenderbufferFormat = GL_DEPTH_COMPONENT24;

    Framebuffer captureFramebuffer(framebufferSpec);

    captureFramebuffer.AttachColorTexture(0, brdfLUT);

    // Save current viewport size
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // configure the viewport to the capture dimensions.
    glViewport(0, 0, 512, 512);

    captureFramebuffer.Bind();

    BRDFLUTShader->use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    RenderPrimitives::RenderQuad();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // configure the viewport to the original framebuffer's screen dimensions
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}