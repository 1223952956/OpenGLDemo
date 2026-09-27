#pragma once
#include "Matrix.h"
#include <glm/mat4x4.hpp>

namespace Math
{
	inline Mat4 FromGlm(const glm::mat4& glmMat)
	{
		Mat4 result;

		for (std::size_t col = 0; col < 4; ++col)
		{
			for (std::size_t row = 0; row < 4; ++row)
			{
				result(row, col) = glmMat[col][row];
			}
		}

		return result;
	}
	inline glm::mat4 ToGlm(const Mat4& mathMat)
	{
		glm::mat4 result(1.0f);

		for (std::size_t col = 0; col < 4; ++col)
		{
			for (std::size_t row = 0; row < 4; ++row)
			{
				result[col][row] = mathMat(row, col);
			}
		}

		return result;
	}
}