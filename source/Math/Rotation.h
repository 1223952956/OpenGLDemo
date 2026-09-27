#pragma once
#include "Quaternion.h"
#include "Vector.h"


namespace Math
{

	class Rotation
	{
	public:
		Rotation() = default;
		Rotation(const Quat& quat) : Value(quat.GetNormalized()) {}
		Rotation(Vec3 axis, float angle);
		Rotation(float pitch, float yaw, float roll);

		Rotation operator*(const Rotation& other) const;

		Vec3 Rotate(const Vec3& vector) const;

		Quat GetQuaternion() const { return Value; }
		Vec3 GetEulerAngles() const;

	private:
		Quat Value;
	};

}