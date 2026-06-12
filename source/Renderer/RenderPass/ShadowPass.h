#pragma once
#include "RenderPass.h"
#include "Renderer/ShadowMapBaker.h"

class ShadowPass :
    public RenderPass
{
public:
    ShadowPass(const std::string& name, ShaderManager& shaderManager);
    void Init(RenderContext& context) override;
    void Resize(unsigned int width, unsigned int height) override;
    void Execute(RenderContext& context) override;

private:
    //TODO
    // Use a different shader
    std::shared_ptr<Shader> PBRShader;

    ShadowMapBaker ShadowBaker;
};

