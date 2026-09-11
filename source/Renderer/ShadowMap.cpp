#include "ShadowMap.h"
#include "Renderer/Framebuffer.h"

ShadowMap2D::ShadowMap2D(uint32_t width, uint32_t height)
	: Width(width)
	, Height(height)
{
    FramebufferSpecification spec;
    spec.Name = "DirectionalLight::ShadowMap";
    spec.Width = width;
    spec.Height = height;
	spec.DepthAttachment.Type = DepthAttachmentType::Texture2D;

    auto& texture2DData = spec.DepthAttachment.Texture;
	texture2DData.Width = width;
	texture2DData.Height = height;
	texture2DData.InternalFormat = GL_DEPTH_COMPONENT;
	texture2DData.DataFormat = GL_DEPTH_COMPONENT;
	texture2DData.DataType = GL_FLOAT;
	texture2DData.SamplerSpec.WrapS = GL_REPEAT;
	texture2DData.SamplerSpec.WrapT = GL_REPEAT;
	texture2DData.SamplerSpec.MinFilter = GL_NEAREST;
	texture2DData.SamplerSpec.MagFilter = GL_NEAREST;
	texture2DData.GenerateMipmaps = false;

	Target = std::make_unique<Framebuffer>(spec);
}
