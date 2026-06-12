#pragma once
#include <string>
#include "RenderContext.h"
#include "Renderer/ShaderManager.h"

class RenderPass
{
public:
	RenderPass(const std::string& name);
	virtual ~RenderPass() = default;

	virtual void Init(RenderContext& context) = 0;
	virtual void Resize(unsigned int width, unsigned int height) = 0;
	virtual void Execute(RenderContext& context) = 0;

	const std::string& GetName() const { return Name; };

protected:
	std::string Name;
};

