#pragma once

namespace Math
{
	class Vec3
	{
	public:
		float x, y, z;
		Vec3() : x(0.f), y(0.f), z(0.f) {}
		Vec3(float value) : x(value), y(value), z(value) {}
		Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

		float GetLengthSquared() const
		{
			return x * x + y * y + z * z;
		}

		float GetLength() const;

		Vec3 operator+(const Vec3& other) const
		{
			return Vec3(x + other.x, y + other.y, z + other.z);
		}

		Vec3 operator-(const Vec3& other) const
		{
			return Vec3(x - other.x, y - other.y, z - other.z);
		}

		Vec3 operator-() const
		{
			return Vec3(-x, -y, -z);
		}

		Vec3 operator*(const Vec3& other) const
		{
			return Vec3(x * other.x, y * other.y, z * other.z);
		}

		Vec3 operator*(float scalar) const
		{
			return Vec3(x * scalar, y * scalar, z * scalar);
		}

		Vec3 GetNormalized() const;

		static Vec3 Cross(const Vec3& a, const Vec3& b);
	};

}

