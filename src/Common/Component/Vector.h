#pragma once
#include <cmath>

struct Vector2f
{
	float x, y;

	Vector2f(const float x, const float y)
		: x(x)
		, y(y)
	{
	}

	Vector2f()
		: Vector2f(0, 0)
	{
	}

	Vector2f operator+(const Vector2f& other) const
	{
		return Vector2f(x + other.x, y + other.y);
	}

	Vector2f operator-(const Vector2f& other) const
	{
		return Vector2f(x - other.x, y - other.y);
	}

	Vector2f operator*(const float scalar) const
	{
		return Vector2f(x * scalar, y * scalar);
	}

	float GetLength() const
	{
		return std::sqrt(x * x + y * y);
	}

	Vector2f Normalized() const
	{
		const float len = GetLength();
		return (len > 0)
			? Vector2f(x / len, y / len)
			: Vector2f();
	}
};

struct Vector3f
{
	float x, y, z;

	Vector3f(const float x, const float y, const float z)
		: x(x)
		, y(y)
		, z(z)
	{
	}

	Vector3f()
		: Vector3f(0, 0, 0)
	{
	}

	Vector3f operator+(const Vector3f& other) const
	{
		return Vector3f(x + other.x, y + other.y, z + other.z);
	}

	Vector3f operator-(const Vector3f& other) const
	{
		return Vector3f(x - other.x, y - other.y, z - other.z);
	}

	Vector3f operator*(const float scalar) const
	{
		return Vector3f(x * scalar, y * scalar, z * scalar);
	}

	float Dot(const Vector3f& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	Vector3f Cross(const Vector3f& other) const
	{
		return Vector3f(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}

	float GetLength() const
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	Vector3f Normalized() const
	{
		const float len = GetLength();
		return (len > 0)
			? Vector3f(x / len, y / len, z / len)
			: Vector3f();
	}
};