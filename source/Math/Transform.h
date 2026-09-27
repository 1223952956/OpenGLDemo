#pragma once

#include "Vector.h"
#include "Rotation.h"
#include "Matrix.h"

namespace Math
{
	class Transform
	{
	public:
		Transform() : Position(0.f, 0.f, 0.f), Rotation(), Scale(1.f, 1.f, 1.f) {}
		Vec3 Position;
		Rotation Rotation;
		Vec3 Scale;

		Mat4 ToMatrix() const;

		Transform operator*(const Transform& other) const;

	private:
		Mat4 TranslationMatrix() const;
		Mat4 RotationMatrix() const;
		Mat4 ScaleMatrix() const;
	};
}




