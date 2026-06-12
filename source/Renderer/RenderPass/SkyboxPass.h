#pragma once
#include "RenderPass.h"
class SkyboxPass :
    public RenderPass
{
public:
    SkyboxPass(const std::string& name, ShaderManager& shaderManager);

    void Init(RenderContext& context) override;
    void Resize(unsigned int width, unsigned int height) override;
    void Execute(RenderContext& context) override;

private:
    std::shared_ptr<Shader> SkyboxShader;

    void UploadCamera(RenderContext& context);
    void DrawSkybox(RenderContext& context);
};

