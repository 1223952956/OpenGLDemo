#pragma once
#include <array>

namespace Math
{
	class Mat4
	{
	public:
		Mat4() : Elements{} {}
		Mat4(const std::array<float, 16>& elements) : Elements(elements) {}

		static Mat4 Identity();

		float& operator()(std::size_t row, std::size_t col)
		{
			return Elements[col * 4 + row];
		}

		float operator()(std::size_t row, std::size_t col) const
		{
			return Elements[col * 4 + row];
		}

		float* Data() { return Elements.data(); }
		const float* Data() const { return Elements.data(); }

		Mat4 operator*(const Mat4& other);

		static Mat4 Transpose(const Mat4& mat);
		//static Mat4 Inverse(const Mat4& mat);

	private:
		std::array<float, 16> Elements;
	};
}

