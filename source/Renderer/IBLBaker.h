#pragma once
#include <memory>

#include "IBLMaterial.h"
#include "Shader.h"


// For now, makes it static
class IBLBaker
{
public:
    static std::unique_ptr<IBLMaterial> Bake(const std::string& path, Shader& shader);

    static void renderCube();

private:
    static GLuint CreateEnvCubemap(GLuint hdrTex, Shader& shader);
    static GLuint CreateIrradianceMap(GLuint envMap);
    static GLuint CreatePrefilterMap(GLuint envMap);
    static GLuint CreateBRDFLUT();


};

