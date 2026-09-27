#include "Rotation.h"
#include <cmath>
#include <algorithm>

namespace Math
{
	Rotation::Rotation(Vec3 axis, float angle)
	{
		const Vec3 normalizedAxis = axis.GetNormalized();
		const float halfAngle = angle * 0.5f;
		const float sinHalfAngle = std::sin(halfAngle);

		Quat quat = Quat(std::cos(halfAngle),
			normalizedAxis.x * sinHalfAngle,
			normalizedAxis.y * sinHalfAngle,
			normalizedAxis.z * sinHalfAngle);

		Value = quat.GetNormalized();
	}
	Rotation::Rotation(float pitch, float yaw, float roll)
	{
		const Rotation pitchRotation(Vec3(1.f, 0.f, 0.f), pitch);
		const Rotation yawRotation(Vec3(0.f, 1.f, 0.f), yaw);
		const Rotation rollRotation(Vec3(0.f, 0.f, 1.f), roll);

		Value = (rollRotation * yawRotation * pitchRotation).GetQuaternion();
	}

	Rotation Rotation::operator*(const Rotation& other) const
	{
		return Rotation(Value * other.Value);
	}

	Vec3 Rotation::Rotate(const Vec3& vector) const
	{
		const Vec3 quaternionVector(Value.x,Value.y,Value.z);
		const Vec3 temp = Vec3::Cross(quaternionVector, vector) * 2.0f;

		return vector + temp * Value.w + Vec3::Cross(quaternionVector, temp);
	}

	Vec3 Rotation::GetEulerAngles() const
	{
		const float w = Value.w;
		const float x = Value.x;
		const float y = Value.y;
		const float z = Value.z;

		const float r00 = 1.0f - 2.0f * (y * y + z * z);
		const float r01 = 2.0f * (x * y - w * z);

		const float r10 = 2.0f * (x * y + w * z);
		const float r11 = 1.0f - 2.0f * (x * x + z * z);

		const float r20 = 2.0f * (x * z - w * y);
		const float r21 = 2.0f * (y * z + w * x);
		const float r22 = 1.0f - 2.0f * (x * x + y * y);

		constexpr float Pi = 3.14159265358979323846f;
		constexpr float Epsilon = 1.0e-6f;

		Vec3 result;

		const float sinYaw = std::clamp(-r20, -1.0f, 1.0f);

		if (std::abs(sinYaw) < 1.0f - Epsilon)
		{
			result.x = std::atan2(r21, r22); // pitch
			result.y = std::asin(sinYaw);    // yaw
			result.z = std::atan2(r10, r00); // roll
		}
		else
		{
			// yaw close to ±90 degrees, gimbal lock occurs
			result.y = std::copysign(Pi * 0.5f, sinYaw);

			// Set roll to zero and calculate pitch based on the remaining rotation
			result.x = 0.0f;
			result.z = std::atan2(-r01, r11);
		}

		return result;
	}
}