#pragma once
#include <string>
#include <vector>

#include "Texture.h"

struct FrameBufferSpecification
{
	std::string Name;

	uint32_t Width;
	uint32_t Height;
};

// TODO
// Bind and attach inside Framebuffer
class Framebuffer
{
public:
	Framebuffer(const std::string& name, unsigned int id);
	~Framebuffer();

	Framebuffer(const Framebuffer&) = delete;
	Framebuffer& operator=(const Framebuffer&) = delete;

	void Bind();
	void UnBind();

	//const std::shared_ptr<Texture2D>& GetColorAttachment(unsigned int index = 0) const 
	//{
	//	if (index >= ColorAttachments.size())
	//	{
	//		return nullptr;
	//	}
	//	return ColorAttachments[index];
	//};

	//const std::shared_ptr<Texture2D> GetDepthAttachment() const
	//{
	//	return DepthAttachment;
	//}

	std::vector<std::shared_ptr<Texture2D>> ColorAttachments;
	std::shared_ptr<Texture2D> DepthAttachment;

private:
	unsigned int ID;
	std::string Name;
};

