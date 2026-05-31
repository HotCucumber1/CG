#pragma once
#include "Vector3.h"

#include <random>

struct Light
{
	Vector3 position;
	Color diffuse;
	Color specular;
	Color ambient;
	float radius;

	Light(const Vector3& pos, const Color& diff, const Color& spec, const Color& amb, float rad = 0)
		: position(pos)
		, diffuse(diff)
		, specular(spec)
		, ambient(amb)
		, radius(rad)
	{
	}

	Vector3 RandomPointOnSurface(std::mt19937& rng) const
	{
		if (radius <= 0)
		{
			return position;
		}

		std::uniform_real_distribution dist(-1.f, 1.f);
		Vector3 p;
		do
		{
			p = Vector3(dist(rng), dist(rng), dist(rng));
		} while (p.Length() > 1);
		return position + p.Normalized() * radius;
	}
};
