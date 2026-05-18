#pragma once
#include <string>
#include <glad/glad.h>

// !!! Never construct data structures directly using Texture2D !!!
class Texture2D
{
public:
	unsigned int Id;
	std::string Path;

    // !!! Delete GPU resource !!!
    ~Texture2D()
    {
        if (Id != 0)
        {
            glDeleteTextures(1, &Id);
        }
    }
};

