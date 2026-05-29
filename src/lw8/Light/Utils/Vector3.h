#pragma once
#include <cmath>

struct Vector3
{
	float x = 0;
	float y = 0;
	float z = 0;

	Vector3() = default;

	Vector3(const float x, const float y, const float z)
		: x(x)
		, y(y)
		, z(z)
	{
	}

	Vector3 operator+(const Vector3& v) const
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(const Vector3& v) const
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator*(const float s) const
	{
		return Vector3(x * s, y * s, z * s);
	}

	Vector3 operator/(const float s) const
	{
		return Vector3(x / s, y / s, z / s);
	}

	Vector3 operator*(const Vector3& v) const
	{
		return Vector3(x * v.x, y * v.y, z * v.z);
	}

	float Dot(const Vector3& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	Vector3 Cross(const Vector3& v) const
	{
		return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	float Length() const
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	Vector3 Normalized() const
	{
		const float len = Length();
		return (len > 0)
			? (*this) / len
			: Vector3();
	}
};

using Color = Vector3;
