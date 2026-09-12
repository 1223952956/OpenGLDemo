#pragma once
#include "RenderPass.h"
#include "Renderer/IBLBaker.h"

// TODO
// Implement deferred rendering to truly split LightingPass and GeometryPass
class LightingPass :
    public RenderPass
{
public:
    LightingPass(const std::string& name, ShaderManager* shaderManager);

    void Init(RenderContext& context) override;
    void Resize(unsigned int width, unsigned int height) override;
    void Execute(RenderContext& context) override;

private:
    std::shared_ptr<Shader> PBRShader;
    IBLBaker IBLBaker;

    void UploadCamera(RenderContext& context);
    void UploadLight(RenderContext& context);

    void DrawPieces(RenderContext& context);
};

