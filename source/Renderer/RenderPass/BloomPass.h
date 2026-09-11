#pragma once
#include "RenderPass.h"

#include "Renderer/Framebuffer.h"

class BloomPass :
    public RenderPass
{
public:
    BloomPass(const std::string& name, ShaderManager& shaderManager);

    void Init(RenderContext& context) override;
    void Resize(unsigned int width, unsigned int height) override;
    void Execute(RenderContext& context) override;

private:
    std::shared_ptr<Shader> GaussianBlurShader;

    std::unique_ptr<Framebuffer> BufferPing;
    std::unique_ptr<Framebuffer> BufferPong;

    void InitPingPongBuffer(uint32_t screenWidth, uint32_t screenHeight);
    void Blur(RenderContext& context);

	FrameBufferSpecification CreateSpecification(uint32_t screenWidth, uint32_t screenHeight, 
        const std::string& framebufferName, const std::string& textureName);
};