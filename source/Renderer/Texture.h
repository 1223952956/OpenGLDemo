#pragma once
#include <iostream>
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
            std::cout << "Delete Texture2D: " << Id << std::endl;
            glDeleteTextures(1, &Id);
        }
    }
};

// !!! Never construct data structures directly using Texture2D !!!
class Cubemap
{
public:
    unsigned int Id;

    // !!! Delete GPU resource !!!
    ~Cubemap()
    {
        if (Id != 0)
        {
            std::cout << "Delete Cubemap: " << Id << std::endl;
            glDeleteTextures(1, &Id);
        }
    }
};

