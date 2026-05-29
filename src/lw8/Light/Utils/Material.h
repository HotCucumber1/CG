#pragma once
#include "Vector3.h"

struct Material
{
	Color diffuse;
	Color specular;
	Color ambient;
	float shininess;

	Material(const Color& diff, const Color& spec, const Color& amb, const float shin)
		: diffuse(diff)
		, specular(spec)
		, ambient(amb)
		, shininess(shin)
	{
	}
};
