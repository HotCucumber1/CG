#pragma once
#include "Utils/Light.h"
#include "Utils/Ray.h"
#include "Utils/Sphere.h"

#include <chrono>
#include <vector>

class RayTracer
{
public:
	RayTracer(int width, int height);

	void AddSphere(const Sphere& sphere);

	void AddLight(const Light& light);

	void Render(std::vector<unsigned char>& pixels) const;

	void SetCamera(const Vector3& pos, const Vector3& lookAt, const Vector3& up, float fov);

	void SetBackground(const Color& bg);

	void SetSoftShadowsEnabled(bool enable);

	void SetLightSamplesCount(const int count)
	{
		m_lightSamplesCount = std::max(1, count);
	}

	void ClearLights()
	{
		m_lights.clear();
	}

private:
	bool Intersect(const Ray& ray, float& t, Sphere*& hitSphere, Vector3& hitNormal) const;

	Color TraceRay(const Ray& ray) const;

	Color ComputeLighting(const Vector3& point, const Vector3& normal, const Vector3& viewDir, const Material& material) const;

	bool IsInShadow(const Vector3& point, const Vector3& normal, const Light& light) const;

	float ShadowFactor(const Vector3& point, const Vector3& normal, const Light& light) const;

private:
	int m_width;
	int m_height;
	Vector3 m_camPos = { 0, 0, 5 };
	Vector3 m_camDir = { 0, 0, -1 };
	Vector3 m_camUp = { 0, 1, 0 };
	Vector3 m_camRight;
	float m_fov = 60;
	Color m_background = { 0, 0, 0 };

	std::vector<Sphere> m_spheres;
	std::vector<Light> m_lights;

	bool m_softShadowsEnabled = false;
	int m_lightSamplesCount = 16;
	mutable std::mt19937 m_rng;
};
