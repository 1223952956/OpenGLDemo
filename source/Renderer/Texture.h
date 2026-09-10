#pragma once
#include <iostream>
#include <string>
#include <glad/glad.h>


class Texture
{
public:
	std::string DebugName;

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
};

class Texture2D : public Texture
{
public:
	Texture2D() = default;

	void Bind()
	{
		glBindTexture(GL_TEXTURE_2D, ID);
	}

	
};

class Cubemap : public Texture
{
public:
	Cubemap() = default;

	void Bind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
	}
};

