#pragma once
#include <iostream>
#include <string>
#include <glad/glad.h>


struct Texture2DData
{
	const void* Data = nullptr;
	uint32_t Width = 0;
	uint32_t Height = 0;	

	GLenum InternalFormat = GL_RGBA8;
	GLenum DataFormat = GL_RGBA;
	GLenum DataType = GL_UNSIGNED_BYTE;
	GLenum WarpParam = GL_REPEAT;
	GLenum MinFilter = GL_LINEAR_MIPMAP_LINEAR;
	GLenum MagFilter = GL_LINEAR;

	bool GenerateMipmaps = true;

	std::string DebugName;
};

class Texture
{
public:
    Texture()
    {
		glGenTextures(1, &ID);
    }
    ~Texture()
    {
        if (ID)
        {
            std::cout << "~Texture() Delete Texture: " << ID << std::endl;
            glDeleteTextures(1, &ID);
        }
    }

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

	Texture(Texture&& other) noexcept
		: ID(other.ID)
        , DebugName(std::move(other.DebugName))
	{
		other.ID = 0;
	}
	Texture& operator=(Texture&& other) noexcept
	{
		if (this != &other)
		{
			if (ID)
			{
				std::cout << "Texture Move Assignment: Delete Texture: " << ID << std::endl;
				glDeleteTextures(1, &ID);
			}
			ID = other.ID;
			other.ID = 0;

            DebugName = std::move(other.DebugName);
		}
		return *this;
	}

	GLuint GetID() const { return ID; }

	void SetDebugName(const std::string& name) { DebugName = name; }
	std::string GetDebugName() const { return DebugName; }

protected:
	GLuint ID = 0;
	std::string DebugName;
};

class Texture2D : public Texture
{
public:
	Texture2D() = default;

	void Bind() { glBindTexture(GL_TEXTURE_2D, ID); }

	void SetData(const Texture2DData& data);
};	

class Cubemap : public Texture
{
public:
	Cubemap() = default;

	void Bind() { glBindTexture(GL_TEXTURE_CUBE_MAP, ID); }
};

