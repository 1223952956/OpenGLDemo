#pragma once
#include <string>

#include "Texture.h"
#include "Shader.h"

class Material
{
public:
    Texture2D* BaseColorTexture = nullptr;
    Texture2D* NormalTexture = nullptr;
    Texture2D* MetallicRoughnessTexture = nullptr;
    Texture2D* EmissiveTexture = nullptr;
    Texture2D* OcclusionTexture = nullptr;

    void Bind(Shader& shader);
};

