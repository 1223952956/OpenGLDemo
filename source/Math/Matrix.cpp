#include "Matrix.h"

namespace Math
{
	Mat4 Math::Mat4::Identity()
	{
		Mat4 mat;
		mat.Elements = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
		return mat;

	}
	Mat4 Mat4::operator*(const Mat4& other)
	{
		Mat4 result;
		for (std::size_t row = 0; row < 4; ++row)
		{
			for (std::size_t col = 0; col < 4; ++col)
			{
				for (std::size_t k = 0; k < 4; ++k)
				{
					result(row, col) += (*this)(row, k) * other(k, col);
				}
			}
		}
		return result;
	}
	Mat4 Mat4::Transpose(const Mat4& mat)
	{
		Mat4 result;
		for (std::size_t row = 0; row < 4; ++row)
		{
			for (std::size_t col = 0; col < 4; ++col)
			{
				result(row, col) = mat(col, row);
			}
		}
		return result;
	}
}