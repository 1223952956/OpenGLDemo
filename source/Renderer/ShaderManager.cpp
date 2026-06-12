#include "ShaderManager.h"

void ShaderManager::Init()
{
    Load("PBRShader", "content/shaders/Piece.vert", "content/shaders/Piece.frag");
    Load("SkyboxShader", "content/shaders/SkyBox.vert", "content/shaders/SkyBox.frag");
    Load("DebugQuadShader", "content/shaders/DebugQuad.vert", "content/shaders/DebugQuad.frag");

    Load("EquirectToCubemapShader", "content/shaders/Cubemap.vert", "content/shaders/EquirectangularToCubemap.frag");
    Load("IrradianceShader", "content/shaders/Cubemap.vert", "content/shaders/IrradianceMap.frag");
    Load("PrefilterShader", "content/shaders/Cubemap.vert", "content/shaders/PrefilterMap.frag");
    Load("BRDFLUTShader", "content/shaders/BRDFLUT.vert", "content/shaders/BRDFLUT.frag");

    Load("SimpleDepthShader", "content/shaders/SimpleDepth.vert", "content/shaders/SimpleDepth.frag");
}

void ShaderManager::ShutDown()
{
    ShaderMap.clear();
}

std::shared_ptr<Shader> ShaderManager::Load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
{
    auto shader = std::make_shared<Shader>(vertexPath, fragmentPath);
    ShaderMap[name] = shader;
    return shader;
}

std::shared_ptr<Shader> ShaderManager::Get(const std::string& name)
{  
    auto it = ShaderMap.find(name);

    if (it == ShaderMap.end())
    {
        return nullptr;
    }

    return it->second;
}
