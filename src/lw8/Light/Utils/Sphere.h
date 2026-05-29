#pragma once
#include "Material.h"

struct Sphere
{
	Vector3 center;
	float radius;
	Material material;

	Sphere(const Vector3& c, const float r, const Material& m)
		: center(c)
		, radius(r)
		, material(m)
	{
	}
};
