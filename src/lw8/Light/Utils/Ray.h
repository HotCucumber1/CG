#pragma once
#include "Vector3.h"

struct Ray
{
	Vector3 origin;
	Vector3 direction;

	Ray() = default;

	Ray(const Vector3& o, const Vector3& d)
		: origin(o)
		, direction(d.Normalized())
	{
	}
};
