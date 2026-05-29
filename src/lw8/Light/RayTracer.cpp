#include "RayTracer.h"

RayTracer::RayTracer(const int width, const int height)
	: m_width(width)
	, m_height(height)
	, m_rng(std::chrono::steady_clock::now().time_since_epoch().count()) // TODO why
{
	m_camRight = m_camDir.Cross(m_camUp).Normalized();
	m_camUp = m_camRight.Cross(m_camDir).Normalized();
}

void RayTracer::AddSphere(const Sphere& sphere)
{
	m_spheres.push_back(sphere);
}

void RayTracer::AddLight(const Light& light)
{
	m_lights.push_back(light);
}

void RayTracer::Render(unsigned char* pixels) const // TODO нахой указатель
{
	const auto aspect = static_cast<float>(m_width) / m_height;
	const auto scale = std::tan(m_fov * 0.5 * M_PI / 180.0);

	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			const auto px = (2 * (x + 0.5) / m_width - 1.0) * aspect * scale;
			const auto py = (1 - 2 * (y + 0.5) / m_height) * scale;

			const auto direction = (m_camDir + m_camRight * px + m_camUp * py).Normalized();
			Ray ray(m_camPos, direction);

			const auto color = TraceRay(ray);
			const auto r = std::min(255, std::max(0, static_cast<int>(color.x * 255)));
			const auto g = std::min(255, std::max(0, static_cast<int>(color.y * 255)));
			const auto b = std::min(255, std::max(0, static_cast<int>(color.z * 255)));

			const auto index = (y * m_width + x) * 3;
			pixels[index + 0] = static_cast<unsigned char>(r);
			pixels[index + 1] = static_cast<unsigned char>(g);
			pixels[index + 2] = static_cast<unsigned char>(b);
		}
	}
}

void RayTracer::SetCamera(const Vector3& pos, const Vector3& lookAt, const Vector3& up, float fov)
{
	m_camPos = pos;
	m_camDir = (lookAt - pos).Normalized();
	m_camUp = up.Normalized();
	m_camRight = m_camDir.Cross(m_camUp).Normalized();
	m_camUp = m_camRight.Cross(m_camDir).Normalized();
	m_fov = fov;
}

void RayTracer::SetBackground(const Color& bg)
{
	m_background = bg;
}

void RayTracer::addTorus(const Torus& torus)
{
	m_tori.push_back(torus);
}

bool RayTracer::Intersect(const Ray& ray, float& t, Sphere*& hitSphere, Vector3& hitNormal) const
{
	bool hit = false;
	t = std::numeric_limits<float>::max();

	for (const auto& sphere : m_spheres)
	{
		auto oc = ray.origin - sphere.center;
		const auto a = ray.direction.Dot(ray.direction);
		const auto b = 2 * oc.Dot(ray.direction);
		const auto c = oc.Dot(oc) - sphere.radius * sphere.radius;
		const auto discriminant = b * b - 4 * a * c;

		if (discriminant > 0)
		{
			const auto sqrtD = std::sqrt(discriminant);
			const auto t1 = (-b - sqrtD) / (2 * a);
			const auto t2 = (-b + sqrtD) / (2 * a);

			if (t1 > 0.001 && t1 < t)
			{
				t = t1;
				hitSphere = const_cast<Sphere*>(&sphere);
				hit = true;
			}
			else if (t2 > 0.001 && t2 < t)
			{
				t = t2;
				hitSphere = const_cast<Sphere*>(&sphere);
				hit = true;
			}
		}
	}
	if (hit)
	{
		const auto hitPoint = ray.origin + ray.direction * t;
		hitNormal = (hitPoint - hitSphere->center).Normalized();
	}
	return hit;
}

Color RayTracer::TraceRay(const Ray& ray) const
{
	float t;
	Sphere* hitSphere = nullptr;
	Vector3 hitNormal;

	if (Intersect(ray, t, hitSphere, hitNormal))
	{
		const auto hitPoint = ray.origin + ray.direction * t;
		const auto viewDir = (m_camPos - hitPoint).Normalized();
		const auto color = ComputeLighting(hitPoint, hitNormal, viewDir, hitSphere->material);
		return color;
	}
	return m_background;
}

Color RayTracer::ComputeLighting(const Vector3& point, const Vector3& normal, const Vector3& viewDir, const Material& material) const
{
	Color result(0, 0, 0);

	for (const auto& light : m_lights)
	{
		// Ambient
		result = result + material.ambient * light.ambient;

		float factor = shadowFactor(point, normal, light);
		if (factor <= 0)
		{
			continue;
		}

		const auto lightDir = (light.position - point).Normalized();
		const auto diff = std::max(0.f, normal.Dot(lightDir));

		// Shadow ray
		// Ray shadowRay(point + normal * 0.001, lightDir);
		// float shadowT;
		// Sphere* dummySphere = nullptr;
		// Vector3 dummyNormal;
		// const auto inShadow = Intersect(shadowRay, shadowT, dummySphere, dummyNormal);
		//
		// if (inShadow && shadowT < (light.position - point).Length())
		// {
		// 	continue;
		// }

		// Diffuse
		const auto diffuseComp = material.diffuse * light.diffuse * diff * factor;
		result = result + diffuseComp;

		// Specular
		Vector3 reflectDir = (normal * 2 * normal.Dot(lightDir) - lightDir).Normalized();
		float spec = std::max(0.f, reflectDir.Dot(viewDir));
		spec = std::pow(spec, material.shininess);

		const auto specularComp = material.specular * light.specular * spec * factor;
		result = result + specularComp;
	}

	return result;
}

bool RayTracer::isInShadow(const Vector3& point, const Vector3& normal, const Light& light) const
{
	const auto lightDir = (light.position - point).Normalized();
	const Ray shadowRay(point + normal * 0.001f, lightDir);

	float t;
	Sphere* hitSphere = nullptr;
	Vector3 dummyNormal;
	if (Intersect(shadowRay, t, hitSphere, dummyNormal))
	{
		const float lightDistance = (light.position - point).Length();
		return t < lightDistance;
	}
	return false;
}

float RayTracer::shadowFactor(const Vector3& point, const Vector3& normal, const Light& light) const
{
	if (!m_softShadowsEnabled || light.radius <= 0)
	{
		return isInShadow(point, normal, light) ? 0 : 1;
	}

	int hitCount = 0;
	for (int i = 0; i < m_lightSamplesCount; ++i)
	{
		const auto lightPoint = light.randomPointOnSurface(m_rng);
		const auto lightDir = (lightPoint - point).Normalized();
		Ray shadowRay(point + normal * 0.001, lightDir);

		float t;
		Sphere* hitSphere = nullptr;
		Vector3 dummyNormal;
		if (Intersect(shadowRay, t, hitSphere, dummyNormal))
		{
			const float lightDistance = (lightPoint - point).Length();
			if (t >= lightDistance)
			{
				hitCount++;
			}
		}
		else
		{
			hitCount++;
		}
	}
	return static_cast<float>(hitCount) / m_lightSamplesCount;
}

bool RayTracer::intersectAll(const Ray& ray, float& t, Material*& material, Vector3& normal) const
{
	// TODO чет-о хуйня какая-то
	bool hit = false;
	t = std::numeric_limits<float>::max();

	for (const auto& sphere : m_spheres)
	{
		float sphereT;
		Vector3 sphereNormal;
		Sphere* hitSphere = nullptr;
		if (Intersect(ray, sphereT, hitSphere, sphereNormal) && sphereT < t)
		{
			t = sphereT;
			material = &hitSphere->material;
			normal = sphereNormal;
			hit = true;
		}
	}

	for (const auto& torus : m_tori)
	{
		float torusT;
		Vector3 torusNormal;
		if (torus.intersect(ray, torusT, torusNormal) && torusT < t)
		{
			t = torusT;
			material = const_cast<Material*>(&torus.material);
			normal = torusNormal;
			hit = true;
		}
	}

	return hit;
}