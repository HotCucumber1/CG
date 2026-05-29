#include "Torus.h"

Vector3 Torus::transformToLocal(const Vector3& point) const
{
	auto p = point - center;

	p.x /= scale.x;
	p.y /= scale.y;
	p.z /= scale.z;

	const auto cz = std::cos(-rotation.z);
	const auto sz = std::sin(-rotation.z);
	const auto cy = std::cos(-rotation.y);
	const auto sy = std::sin(-rotation.y);
	const auto cx = std::cos(-rotation.x);
	const auto sx = std::sin(-rotation.x);

	const auto x1 = p.x * cz - p.y * sz;
	const auto y1 = p.x * sz + p.y * cz;
	const auto z1 = p.z;

	const auto x2 = x1 * cy + z1 * sy;
	const auto z2 = -x1 * sy + z1 * cy;
	const auto y2 = y1;

	const auto y3 = y2 * cx - z2 * sx;
	const auto z3 = y2 * sx + z2 * cx;
	const auto x3 = x2;

	return { x3, y3, z3 };
}

Vector3 Torus::transformToWorld(const Vector3& localNormal) const
{
	const auto cx = std::cos(rotation.x);
	const auto sx = std::sin(rotation.x);
	const auto cy = std::cos(rotation.y);
	const auto sy = std::sin(rotation.y);
	const auto cz = std::cos(rotation.z);
	const auto sz = std::sin(rotation.z);

	const auto y1 = localNormal.y * cx - localNormal.z * sx;
	const auto z1 = localNormal.y * sx + localNormal.z * cx;
	const auto x1 = localNormal.x;

	const auto x2 = x1 * cy + z1 * sy;
	const auto z2 = -x1 * sy + z1 * cy;
	const auto y2 = y1;

	const auto x3 = x2 * cz - y2 * sz;
	const auto y3 = x2 * sz + y2 * cz;
	const auto z3 = z2;

	const Vector3 worldNormal(x3 / scale.x, y3 / scale.y, z3 / scale.z);
	return worldNormal.Normalized();
}

std::vector<double> solveQuadratic(double a, double b, double c)
{
	std::vector<double> roots;
	const auto discriminant = b * b - 4 * a * c;

	if (discriminant < 0)
	{
		return roots;
	}

	const auto sqrtD = std::sqrt(discriminant);
	roots.push_back((-b - sqrtD) / (2 * a));
	roots.push_back((-b + sqrtD) / (2 * a));

	return roots;
}

std::vector<double> solveCubic(double a, double b, double c, double d)
{
	std::vector<double> roots;
	if (std::abs(a) < 0.00000001)
	{
		return solveQuadratic(b, c, d);
	}

	const auto ba = b / a;
	const auto ca = c / a;
	const auto da = d / a;

	const auto p = ca - ba * ba / 3.0;
	const auto q = da + (2.0 * ba * ba * ba - 9.0 * ba * ca) / 27.0;
	const auto discriminant = (q * q) / 4.0 + (p * p * p) / 27.0;

	if (discriminant > 0)
	{
		const auto sqrtD = sqrt(discriminant);
		const auto u = cbrt(-q / 2.0 + sqrtD);
		const auto v = cbrt(-q / 2.0 - sqrtD);
		roots.push_back(u + v - ba / 3.0);
	}
	else if (std::abs(discriminant) < 0.00000001)
	{
		const auto u = cbrt(-q / 2.0);
		roots.push_back(2.0 * u - ba / 3.0);
		roots.push_back(-u - ba / 3.0);
	}
	else
	{
		const auto r = sqrt(-p * p * p / 27.0);
		const auto phi = acos(-q / (2.0 * r));
		const auto s = 2.0 * cbrt(r);
		roots.push_back(s * cos(phi / 3.0) - ba / 3.0);
		roots.push_back(s * cos((phi + 2.0 * M_PI) / 3.0) - ba / 3.0);
		roots.push_back(s * cos((phi + 4.0 * M_PI) / 3.0) - ba / 3.0);
	}

	return roots;
}

// формула Ferrari (https://ru.wikipedia.org/wiki/%D0%9C%D0%B5%D1%82%D0%BE%D0%B4_%D0%A4%D0%B5%D1%80%D1%80%D0%B0%D1%80%D0%B8)
std::vector<double> Torus::solveQuartic(double a, double b, double c, double d, double e)
{
	std::vector<double> roots;

	if (std::abs(a) < 0.00000001)
	{
		return solveCubic(b, c, d, e);
	}

	const auto ba = b / a;
	const auto ca = c / a;
	const auto da = d / a;
	const auto ea = e / a;

	const auto p = ca - 3.0 * ba * ba / 8.0;
	const auto q = da - ba * ca / 2.0 + ba * ba * ba / 8.0;
	const auto r = ea - ba * da / 4.0 + ba * ba * ca / 16.0 - 3.0 * ba * ba * ba * ba / 256.0;

	const auto cubicRoots = solveCubic(1.0, p / 2.0, (p * p - 4.0 * r) / 16.0, -q * q / 64.0);

	double y = 0;
	for (const auto& root : cubicRoots)
	{
		if (root > 0)
		{
			y = root;
			break;
		}
	}

	const auto sqrtY = sqrt(y);
	const auto k = -q / (8.0 * sqrtY);

	const auto roots1 = solveQuadratic(1.0, sqrtY, y / 2.0 + k);
	const auto roots2 = solveQuadratic(1.0, -sqrtY, y / 2.0 - k);

	for (const auto& root : roots1)
	{
		roots.push_back(root);
	}
	for (const auto& root : roots2)
	{
		roots.push_back(root);
	}
	for (double& root : roots)
	{
		root -= ba / 4.0;
	}
	return roots;
}

std::vector<double> Torus::getIntersectionParams(const Ray& localRay) const
{
	const auto& o = localRay.origin;
	const auto& d = localRay.direction;

	const auto R2 = R * R;
	const auto r2 = r * r;
	const auto rDiff = R2 - r2;

	const auto xd2 = d.x * d.x;
	const auto yd2 = d.y * d.y;
	const auto zd2 = d.z * d.z;
	const auto d2 = xd2 + yd2 + zd2;

	const auto xo2 = o.x * o.x;
	const auto yo2 = o.y * o.y;
	const auto zo2 = o.z * o.z;
	const auto o2 = xo2 + yo2 + zo2;

	const auto xOyd = o.x * d.x;
	const auto yOyd = o.y * d.y;
	const auto zOyd = o.z * d.z;
	const auto odOtd = xOyd + yOyd + zOyd;

	const auto A = d2 * d2;
	const auto B = 4.0 * d2 * odOtd;
	const auto C = 4.0 * odOtd * odOtd + 2.0 * d2 * (o2 + rDiff) - 4.0 * R2 * (xd2 + yd2);
	const auto D = 4.0 * odOtd * (o2 + rDiff) - 8.0 * R2 * (o.x * d.x + o.y * d.y);
	const auto E = (o2 + rDiff) * (o2 + rDiff) - 4.0 * R2 * (xo2 + yo2) - 4.0 * R2 * r2;

	return solveQuartic(A, B, C, D, E);
}

Vector3 Torus::getNormal(const Vector3& localPoint) const
{
	double x = localPoint.x;
	double y = localPoint.y;
	double z = localPoint.z;

	double R2 = R * R;
	double r2 = r * r;

	double F = x * x + y * y + z * z + R2 - r2;
	double dfdx = 2.0 * x * F - 4.0 * R2 * x;
	double dfdy = 2.0 * y * F - 4.0 * R2 * y;
	double dfdz = 2.0 * z * F;

	return Vector3(dfdx, dfdy, dfdz).Normalized();
}

bool Torus::intersect(const Ray& ray, float& t, Vector3& normal) const
{
	Ray localRay;
	localRay.origin = transformToLocal(ray.origin);
	localRay.direction = ray.direction;

	const auto roots = getIntersectionParams(localRay);

	float minT = std::numeric_limits<float>::max();
	for (const auto& root : roots)
	{
		if (root > 0.00000001
			&& root < minT)
		{
			minT = static_cast<float>(root);
		}
	}

	if (minT == std::numeric_limits<float>::max())
	{
		return false;
	}

	t = minT;
	const auto localPoint = localRay.origin + localRay.direction * t;
	const auto localNormal = getNormal(localPoint);
	normal = transformToWorld(localNormal);

	return true;
}
