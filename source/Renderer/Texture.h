#pragma once
#include <iostream>
#include <string>
#include <glad/glad.h>

class Texture
{
public:
    GLuint ID = 0;

    virtual ~Texture()
    {
        if (ID)
        {
            std::cout << "~Texture() Delete Texture: " << ID << std::endl;
            glDeleteTextures(1, &ID);
        }
    }

    Texture() = default;

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept
    {
        ID = other.ID;
        other.ID = 0;
    }

    Texture& operator=(Texture&& other) noexcept
    {
        if (this != &other)
        {
            if (ID)
            {
                std::cout << "Move assignment Delete Texture: " << ID << std::endl;
                glDeleteTextures(1, &ID);
            }

            ID = other.ID;
            other.ID = 0;
        }

        return *this;
    }
};

// !!! Never construct data structures directly using Texture2D !!!
class Texture2D : public Texture
{
public:
	std::string Path;
};

// !!! Never construct data structures directly using Cubemap !!!
class Cubemap : public Texture
{
public:
    std::string Name;
};

