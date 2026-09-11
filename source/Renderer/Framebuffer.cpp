#include "Framebuffer.h"


Framebuffer::Framebuffer(const FrameBufferSpecification& spec)
	: ID(0)
	, DepthRenderBufferID(0)
	, Spec(spec)
	, DebugName(spec.Name)
{
	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	// Create color attachments
	for (const auto& colorAttachment : spec.ColorAttachments)
	{
		ColorAttachments.emplace_back(std::make_unique<Texture2D>());
		ColorAttachments.back()->SetData(colorAttachment);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, 
			GL_COLOR_ATTACHMENT0 + (GLenum)ColorAttachments.size() - 1,
			GL_TEXTURE_2D, 
			ColorAttachments.back()->GetID(),
			0);
	}

	// Create depth attachment
	switch (spec.DepthAttachment.Type)
	{
	case DepthAttachmentType::Renderbuffer:
	{
		glGenRenderbuffers(1, &DepthRenderBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, DepthRenderBufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, spec.DepthAttachment.RenderbufferFormat, spec.Width, spec.Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthRenderBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		break;
	}

	case DepthAttachmentType::Texture2D:
	{
		DepthAttachment = std::make_unique<Texture2D>();
		DepthAttachment->SetData(spec.DepthAttachment.Texture);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthAttachment->GetID(), 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		break;
	}
	case DepthAttachmentType::None:
	default:
		break;
	}

	// Set the list of draw buffers.
	const size_t count = ColorAttachments.size();
	if (count > 0)
	{
		std::vector<GLenum> attachments;
		attachments.reserve(count);

		for (GLenum i = 0; i < count; ++i)
		{
			attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
		}

		glDrawBuffers((GLsizei)count, attachments.data());
	}

	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::Framebuffer(const std::string& name, unsigned int id)
	: DebugName(name)
	, ID(id)
	, DepthRenderBufferID(0)
{
}

Framebuffer::~Framebuffer()
{
	if (ID)
	{
		std::cout << "~Framebuffer() Delete Framebuffer: " << DebugName << std::endl;
		glDeleteFramebuffers(1, &ID);
	}
	if (DepthRenderBufferID)
	{
		std::cout << "~Framebuffer() Delete DepthRenderBuffer: " << DebugName << std::endl;
		glDeleteRenderbuffers(1, &DepthRenderBufferID);
	}
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void Framebuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint Framebuffer::GetDepthMap() const
{
	switch (Spec.DepthAttachment.Type)
	{
	case DepthAttachmentType::Texture2D:
		return DepthAttachment->GetID();
	case DepthAttachmentType::Renderbuffer:
		return DepthRenderBufferID;
	default:
		return 0;
	}
}
