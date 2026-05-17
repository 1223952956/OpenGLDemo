#pragma once
#ifndef SHADER_H
#define SHADER_H
#endif // !SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
	// 程序ID
	unsigned int ID;

	// 构造器读取并构造着色器
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	// 使用/激活程序
	void use();
	// uniform工具函数
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, float x, float y, float z);
	void setVec3(const std::string& name, const glm::vec3& value);
	void setMat4(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat* value);
};

