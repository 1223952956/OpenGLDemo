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

    std::shared_ptr<Framebuffer> BufferPing;
    std::shared_ptr<Framebuffer> BufferPong;

    void InitPingPongBuffer(uint32_t screenWidth, uint32_t screenHeight);
    void Blur(RenderContext& context);
};