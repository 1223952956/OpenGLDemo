#include "Framebuffer.h"


Framebuffer::Framebuffer(const std::string& name, unsigned int id)
	:Name(name),
	ID(id)
{
}

Framebuffer::~Framebuffer()
{
	if (ID)
	{
		std::cout << "~Framebuffer() Delete Framebuffer: " << Name << std::endl;
		glDeleteFramebuffers(1, &ID);
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
