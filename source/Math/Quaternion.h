#pragma once


namespace Math
{
	class Quat
	{
	public:
		float w, x, y, z;
		Quat() : w(1.f), x(0.f), y(0.f), z(0.f) {}
		Quat(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

		float lengthSquared() const
		{
			return w * w + x * x + y * y + z * z;
		}

		float length() const;

		Quat GetNormalized() const;

		Quat operator*(const Quat& other) const;
	};
}
