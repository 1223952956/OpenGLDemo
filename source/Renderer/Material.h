#pragma once
#include <string>

#include "Texture.h"
#include "Shader.h"

class Material
{
public:
    // base color
    Texture2D* BaseColorTexture = nullptr;
    glm::vec4 BaseColorFactor = glm::vec4(1.0f);

    // normal
    Texture2D* NormalTexture = nullptr;
    float NormalScale = 1.0f;

    // metallic and roughness
    Texture2D* MetallicRoughnessTexture = nullptr;
    float MetallicFactor = 1.0f;
    float RoughnessFactor = 1.0f;

    // emissive
    Texture2D* EmissiveTexture = nullptr;
    glm::vec3 EmissiveFactor = glm::vec3(1.0f);

    // occlusion
    Texture2D* OcclusionTexture = nullptr;
    float OcclusionStrength = 1.0f;

    // alpha
    bool IsOpaque = true;
    float AlphaCutoff = 0.5f;

    void Bind(Shader& shader);
};

