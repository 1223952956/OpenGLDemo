#include "Transform.h"

namespace Math
{
	Mat4 Math::Transform::ToMatrix() const
	{
		return TranslationMatrix() * RotationMatrix() * ScaleMatrix();
	}
	Transform Transform::operator*(const Transform& other) const
	{
		Transform result;

		result.Rotation = Rotation * other.Rotation;
		result.Scale = Scale * other.Scale;
		result.Position = Position + Rotation.Rotate(other.Position * Scale);
		
		return result;
	}
	Mat4 Transform::TranslationMatrix() const
	{
		std::array<float, 16> elements = {
			1.f,		0.f,		0.f,		0.f,
			0.f,		1.f,		0.f,		0.f,
			0.f,		0.f,		1.f,		0.f,
			Position.x, Position.y, Position.z, 1.f
		};
		return Mat4(elements);
	}
	Mat4 Transform::RotationMatrix() const
	{
		const float w = Rotation.GetQuaternion().w;
		const float x = Rotation.GetQuaternion().x;
		const float y = Rotation.GetQuaternion().y;
		const float z = Rotation.GetQuaternion().z;

		const float xx = x * x;
		const float yy = y * y;
		const float zz = z * z;

		const float xy = x * y;
		const float xz = x * z;
		const float yz = y * z;

		const float wx = w * x;
		const float wy = w * y;
		const float wz = w * z;

		std::array<float, 16> elements = {
			1.f - 2.f * (yy + zz), 2.f * (xy + wz),       2.f * (xz - wy),       0.f,
			2.f * (xy - wz),       1.f - 2.f * (xx + zz), 2.f * (yz + wx),       0.f,
			2.f * (xz + wy),       2.f * (yz - wx),       1.f - 2.f * (xx + yy), 0.f,
			0.f,                   0.f,                   0.f,                   1.f
		};

		return Mat4(elements);
	}
	Mat4 Transform::ScaleMatrix() const
	{
		std::array<float, 16> elements = {
			Scale.x, 0.f,	  0.f,     0.f,
			0.f,	 Scale.y, 0.f,     0.f,
			0.f,	 0.f,     Scale.z, 0.f,
			0.f,	 0.f,     0.f,     1.f
		};
		return Mat4(elements);
	}
}

