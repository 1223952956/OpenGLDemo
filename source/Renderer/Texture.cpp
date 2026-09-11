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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, data.WarpParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, data.WarpParam);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, data.MinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, data.MagFilter);

	if (data.GenerateMipmaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	DebugName = data.DebugName;

	glBindTexture(GL_TEXTURE_2D, 0);
}
