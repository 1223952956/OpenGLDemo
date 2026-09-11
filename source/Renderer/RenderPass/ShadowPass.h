#pragma once
#include "RenderPass.h"

class ShadowPass :
    public RenderPass
{
public:
    ShadowPass(const std::string& name, ShaderManager& shaderManager);
    void Init(RenderContext& context) override;
    void Resize(unsigned int width, unsigned int height) override;
    void Execute(RenderContext& context) override;

private:
    void Bake(Scene* scene);

    glm::mat4 ComputeLightSpaceMatrix(glm::vec3 lightPos);
    void DrawPieces(Scene* scene);
    //TODO
    // Use a different shader
    std::shared_ptr<Shader> PBRShader;
    std::shared_ptr<Shader> SimpleDepthShader;

    std::vector<std::unique_ptr<ShadowMap2D>> DirectionalShadows;
};

