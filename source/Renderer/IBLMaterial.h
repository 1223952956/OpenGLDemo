#pragma once
#include <memory>

#include "Texture.h"
#include "Shader.h"

class IBLMaterial
{
public:
 	std::unique_ptr<Cubemap> EnvCubeMap;
	std::unique_ptr<Cubemap> IrradianceMap;
	std::unique_ptr<Cubemap> PrefilterMap;
	std::unique_ptr<Texture2D> BRDFLUT;

	void Upload(Shader& shader);
	void Bind();
};

