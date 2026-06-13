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
}

void BloomPass::Execute(RenderContext& context)
{
    glActiveTexture(GL_TEXTURE0);
    Blur(context);
}

void BloomPass::InitPingPongBuffer(uint32_t screenWidth, uint32_t screenHeight)
{
    GLuint pingpongFBO[2];
    GLuint pingpongBuffer[2];
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongBuffer);
    for (GLuint i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0
        );
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "Framebuffer not complete!" << std::endl;
        }
    }

    BufferPing = std::make_shared<Framebuffer>("BloomPass::BufferPing", pingpongFBO[0]);
    BufferPong = std::make_shared<Framebuffer>("BloomPass::BufferPong", pingpongFBO[1]);

    auto blurPing = std::make_shared<Texture2D>("BloomPass::BlurPing", pingpongBuffer[0]);
    auto blurPong = std::make_shared<Texture2D>("BloomPass::BlurPong", pingpongBuffer[1]);

    BufferPing->ColorAttachments = { blurPing };
    BufferPong->ColorAttachments = { blurPong };

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

        GLuint sourceTexture = (i == 0) ? context.SceneFramebuffer->ColorAttachments[1]->ID : source->ColorAttachments[0]->ID;
        glBindTexture(GL_TEXTURE_2D, sourceTexture);

        RenderPrimitives::RenderQuad();

        isHorizontal = !isHorizontal;
    }

    context.BlurColor = isHorizontal ? BufferPing->ColorAttachments[0] : BufferPong->ColorAttachments[0];

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}