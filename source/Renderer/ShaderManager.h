#pragma once
#include <unordered_map>
#include <memory>

#include "Shader.h"


class ShaderManager
{
public:
	void Init();
	void ShutDown();

	std::shared_ptr<Shader> Load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
	std::shared_ptr<Shader> Get(const std::string& name);
private:
	std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderMap;
};

