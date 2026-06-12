#pragma once
#include "RenderPass.h"
class GeometryPass :
    public RenderPass
{
public:
    GeometryPass(const std::string& name, ShaderManager& shaderManager);

    void Init(RenderContext& context) override;
    void Resize(unsigned int width, unsigned int height) override;
    void Execute(RenderContext& context) override;

private:
    std::shared_ptr<Shader> PBRShader;
};

