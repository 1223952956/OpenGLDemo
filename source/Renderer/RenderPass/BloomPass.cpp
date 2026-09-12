#include "BloomPass.h"
#include "Renderer/RenderPrimitives.h"

BloomPass::BloomPass(const std::string& name, ShaderManager& shaderManager)
	:RenderPass(name)
{
    GaussianBlurShader = shaderManager.Get("GaussianBlurShader");
}

void BloomPass::Init(RenderContext& context)
{
    GaussianBlurShader->use();
    GaussianBlurShader->setInt("image", 0);
    InitPingPongBuffer(context.ScreenWidth, context.ScreenHeight);
}

void BloomPass::Resize(unsigned int width, unsigned int height)
{
    if (BufferPing)
    {
		BufferPing->Resize(width, height);
    }
    if (BufferPong)
    {
		BufferPong->Resize(width, height);
    }
}

void BloomPass::Execute(RenderContext& context)
{
    glActiveTexture(GL_TEXTURE0);
    Blur(context);
}

void BloomPass::InitPingPongBuffer(uint32_t screenWidth, uint32_t screenHeight)
{
    BufferPing = std::make_unique<Framebuffer>(CreateSpecification(screenWidth, screenHeight, 
        "BloomPass::BufferPing", "BloomPass::BlurPing"));
    BufferPong = std::make_unique<Framebuffer>(CreateSpecification(screenWidth, screenHeight, 
        "BloomPass::BufferPong", "BloomPass::BlurPong"));
}

void BloomPass::Blur(RenderContext& context)
{
    GaussianBlurShader->use();
    constexpr GLuint blurPasses = 10;

    bool isHorizontal = true;
    for (GLuint i = 0; i < blurPasses; ++i)
    {
        auto& target = isHorizontal ? BufferPong : BufferPing;
        auto& source = isHorizontal ? BufferPing : BufferPong;

        target->Bind();
        GaussianBlurShader->setBool("is_horizontal", isHorizontal);

		if (i == 0)
		{
			context.SceneFramebuffer->ColorAttachments[1]->Bind();
		}
		else
		{
			source->ColorAttachments[0]->Bind();
		}

        RenderPrimitives::RenderQuad();

        isHorizontal = !isHorizontal;
    }

    context.BlurColor = isHorizontal ? BufferPing->ColorAttachments[0].get() : BufferPong->ColorAttachments[0].get();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FramebufferSpecification BloomPass::CreateSpecification(uint32_t screenWidth, uint32_t screenHeight, const std::string& framebufferName, const std::string& textureName)
{
    FramebufferSpecification spec;
    spec.Name = framebufferName;
    spec.Width = screenWidth;
    spec.Height = screenHeight;

    spec.ColorAttachments.emplace_back();
    auto& colorAttachment = spec.ColorAttachments.back();

    colorAttachment.Width = screenWidth;
    colorAttachment.Height = screenHeight;
    colorAttachment.InternalFormat = GL_RGBA16F;
    colorAttachment.DataFormat = GL_RGBA;
    colorAttachment.DataType = GL_FLOAT;
    colorAttachment.SamplerSpec.WrapS = GL_CLAMP_TO_EDGE;
    colorAttachment.SamplerSpec.WrapT = GL_CLAMP_TO_EDGE;
    colorAttachment.SamplerSpec.MinFilter = GL_LINEAR;
    colorAttachment.SamplerSpec.MagFilter = GL_LINEAR;
    colorAttachment.GenerateMipmaps = false;
    colorAttachment.DebugName = textureName;

    return spec;
}
