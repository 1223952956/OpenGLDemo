#pragma once
#include <iostream>
#include <string>
#include <glad/glad.h>

class Texture
{
public:
    unsigned int ID = 0;

    virtual ~Texture()
    {
        if (ID)
        {
            std::cout << "~Texture() Delete Texture: " << ID << std::endl;
            glDeleteTextures(1, &ID);
        }
    }
    Texture() = default;
    Texture(unsigned int id) 
        :ID(id)
    {
    }

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
};

class Texture2D : public Texture
{
public:
    Texture2D() = default;
    Texture2D(const std::string& path, unsigned int id)
        :Texture(id),
        Path(path)
    {
    }

	std::string Path;
};

class Cubemap : public Texture
{
public:
    std::string Name;
};

