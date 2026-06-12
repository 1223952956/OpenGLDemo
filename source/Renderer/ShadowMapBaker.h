#pragma once
#include <memory>

#include "ShadowMap.h"

#include "Scene.h"
#include "Shader.h"
#include "Light/DirectionalLight.h"
#include "ShaderManager.h"

class ShadowMapBaker
{
public:
	ShadowMapBaker(ShaderManager& shaderManager);
	void Init(Scene* scene);
	void Bake(Scene* scene);
private:
	glm::mat4 ComputeLightSpaceMatrix(glm::vec3 lightPos);
	void DrawPieces(Scene* scene);

	std::shared_ptr<Shader> SimpleDepthShader;
};

