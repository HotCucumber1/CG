#pragma once
#include "Material.h"
#include "Ray.h"
#include "Vector3.h"
#include <vector>

struct Torus
{
	Vector3 center;
	float R;
	float r;
	Material material;
	Vector3 rotation = { 0, 0, 0 };
	Vector3 scale = { 1, 1, 1 };

	Torus(const Vector3& c,
		const float bigR,
		const float smallR,
		const Material& mat)
		: center(c)
		, R(bigR)
		, r(smallR)
		, material(mat)
	{
	}

	Vector3 transformToLocal(const Vector3& point) const;

	Vector3 transformToWorld(const Vector3& localNormal) const;

	static std::vector<double> solveQuartic(double a, double b, double c, double d, double e);

	bool intersect(const Ray& ray, float& t, Vector3& normal) const;

private:
	std::vector<double> getIntersectionParams(const Ray& localRay) const;

	Vector3 getNormal(const Vector3& localPoint) const;
};