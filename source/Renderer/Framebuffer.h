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

struct FramebufferSpecification
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
	Framebuffer(const FramebufferSpecification& spec);
	~Framebuffer();

	Framebuffer(const Framebuffer&) = delete;
	Framebuffer& operator=(const Framebuffer&) = delete;

	void Bind();
	void UnBind();

	void Resize(uint32_t width, uint32_t height);
	void ResizeDepthRenderbuffer(uint32_t width, uint32_t height);

	void AttachColorTexture(uint32_t index, Texture2D* data);
	void AttachCubemapFace(uint32_t index, Cubemap* data, uint32_t face, uint32_t mipLevel = 0);

	GLuint GetID() const { return ID; }
	GLuint GetDepthMap() const;

	std::vector<std::unique_ptr<Texture2D>> ColorAttachments;
	std::unique_ptr<Texture2D> DepthAttachment;

private:
	GLuint ID;
	GLuint DepthRenderBufferID;

	FramebufferSpecification Spec;

	std::string DebugName;
};

