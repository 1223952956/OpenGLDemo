#include "Texture.h"

void Texture2D::SetData(const Texture2DData& data)
{
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexImage2D(GL_TEXTURE_2D, 0, 
		data.InternalFormat, 
		data.Width, 
		data.Height, 
		0, 
		data.DataFormat, 
		data.DataType, 
		data.Data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, data.SamplerSpec.WrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, data.SamplerSpec.WrapT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, data.SamplerSpec.MinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, data.SamplerSpec.MagFilter);

	if (data.GenerateMipmaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	DebugName = data.DebugName;

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Cubemap::SetData(const CubemapData& data)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

	for (uint32_t mip = 0; mip < data.MipLevels; ++mip)
	{
		uint32_t mipWidth = std::max(1u, data.Width >> mip);
		uint32_t mipHeight = std::max(1u, data.Height >> mip);

		for (unsigned int face = 0; face < 6; ++face)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 
				mip,
				data.InternalFormat,
				mipWidth,
				mipHeight,
				0,
				data.DataFormat,
				data.DataType,
				nullptr);
		}
	}

	// Not a good way to determine if mipmaps are generated
	if (data.MipLevels > 1)
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, data.MipLevels - 1);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, data.SamplerSpec.WrapS);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, data.SamplerSpec.WrapT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, data.SamplerSpec.WrapR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, data.SamplerSpec.MinFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, data.SamplerSpec.MagFilter);

	DebugName = data.DebugName;

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
