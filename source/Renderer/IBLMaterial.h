#pragma once
#include <memory>

#include "Texture.h"
#include "Shader.h"

class IBLMaterial
{
public:
 	Cubemap EnvCubeMap;
	Cubemap IrradianceMap;
	Cubemap PrefilterMap;
	Texture2D BRDFLUT;

	void Bind(Shader& shader);
};

