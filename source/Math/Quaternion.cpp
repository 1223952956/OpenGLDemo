#include "Quaternion.h"
#include <cmath>

namespace Math
{
    float Math::Quat::length() const
    {
		return std::sqrt(lengthSquared());
    }

    Quat Quat::GetNormalized() const
    {
		Quat result = *this;
        float len = length();
		if (len <= 1.0e-6f)
		{
			return Quat(1.f, 0.f, 0.f, 0.f);
		}

		result.w /= len;
		result.x /= len;
		result.y /= len;
		result.z /= len;

        return result;
    }
    Quat Quat::operator*(const Quat& other) const
    {
		return Quat(
			w * other.w - x * other.x - y * other.y - z * other.z,
			w * other.x + x * other.w + y * other.z - z * other.y,
			w * other.y - x * other.z + y * other.w + z * other.x,
			w * other.z + x * other.y - y * other.x + z * other.w
		);
    }
}


