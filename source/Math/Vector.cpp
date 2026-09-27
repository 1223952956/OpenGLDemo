#include "Vector.h"
#include <cmath>

namespace Math
{
	float Vec3::GetLength() const
	{
		{
			return std::sqrt(GetLengthSquared());
		}
	}
	Vec3 Math::Vec3::GetNormalized() const
	{
		float length = GetLength();
		if (length > 0.0f)
		{
			return Vec3(x / length, y / length, z / length);
		}
		return Vec3(0.f, 0.f, 0.f);
	}
	Vec3 Vec3::Cross(const Vec3& a, const Vec3& b)
	{
		return Vec3(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		);
	}
}