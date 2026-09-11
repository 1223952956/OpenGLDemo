#pragma once
#include <string>
#include <vector>
#include <optional>

#include "Texture.h"

enum class DepthAttachmentType
{
	None,
	Texture2D,
	Renderbuffer
};

struct DepthAttachmentSpecification
{
	DepthAttachmentType Type = DepthAttachmentType::None;
	Texture2DData Texture;
	GLenum RenderbufferFormat = GL_DEPTH_COMPONENT;
};

struct FrameBufferSpecification
{
	std::string Name;

	uint32_t Width = 1;
	uint32_t Height = 1;

	std::vector<Texture2DData> ColorAttachments;
	DepthAttachmentSpecification DepthAttachment;
};

// TODO
// Bind and attach inside Framebuffer
class Framebuffer
{
public:
	Framebuffer(const FrameBufferSpecification& spec);
	Framebuffer(const std::string& name, unsigned int id);
	~Framebuffer();

	Framebuffer(const Framebuffer&) = delete;
	Framebuffer& operator=(const Framebuffer&) = delete;

	void Bind();
	void UnBind();

	GLuint GetID() const { return ID; }
	GLuint GetDepthMap() const;

	std::vector<std::unique_ptr<Texture2D>> ColorAttachments;
	std::unique_ptr<Texture2D> DepthAttachment;

private:
	GLuint ID;
	GLuint DepthRenderBufferID;

	FrameBufferSpecification Spec;

	std::string DebugName;
};

