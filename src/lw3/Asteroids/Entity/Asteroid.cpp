#include "Asteroid.h"

#include <algorithm>
#include <cmath>

#include "../../../Common/Random/Random.h"

bool PointInPolygon(double x, double y, const std::vector<Point>& polygon);
std::vector<Asteroid> CreateTwoAsteroids(
	const Point& pos,
	double baseSize,
	AsteroidSize newSize);

Asteroid::Asteroid(
	const Point center,
	const Vector velocity,
	const AsteroidSize type,
	const double baseSize)
	: m_pos(center)
	, m_velocity(velocity)
	, m_baseSize(baseSize)
	, m_size(type)
{
	GenerateShape();
}

void Asteroid::Update()
{
	m_pos.x += m_velocity.x;
	m_pos.y += m_velocity.y;
}

void Asteroid::BoundPosition(const int width, const int height)
{
	if (m_pos.x < 0)
	{
		m_pos.x = width;
	}
	if (m_pos.x > width)
	{
		m_pos.x = 0;
	}
	if (m_pos.y < 0)
	{
		m_pos.y = height;
	}
	if (m_pos.y > height)
	{
		m_pos.y = 0;
	}
}

Point Asteroid::GetPosition() const
{
	return m_pos;
}

std::vector<Point> Asteroid::GetLocalPoints() const
{
	return m_localPoints;
}

AsteroidSize Asteroid::GetSize() const
{
	return m_size;
}

bool Asteroid::IsPointInside(const Point& point) const
{
	const double localX = point.x - m_pos.x;
	const double localY = point.y - m_pos.y;

	return PointInPolygon(localX, localY, m_localPoints);
}

std::vector<Asteroid> Asteroid::Split() const
{
	switch (m_size)
	{
	case AsteroidSize::Large: {
		return CreateTwoAsteroids(m_pos, m_baseSize / 2, AsteroidSize::Medium);
	}
	// case AsteroidSize::Medium: {
	// 	return CreateTwoAsteroids(m_pos, m_baseSize / 2, AsteroidSize::Small);
	// }
	case AsteroidSize::Small:
	default:
		return {};
	}
}

void Asteroid::GenerateShape()
{
	constexpr int minPoints = 8;
	constexpr double maxDeviation = M_PI / 12;
	m_localPoints.clear();

	const int numPoints = minPoints + GetRandomInt(0, 7);

	std::vector<double> angles;
	const double angleStep = 2.0 * M_PI / numPoints;

	for (int i = 0; i < numPoints; i++)
	{
		const double baseAngle = i * angleStep;
		const double randomOffset = (GetRandomDouble(-1.0, 1.0)) * maxDeviation;
		angles.push_back(baseAngle + randomOffset);
	}

	std::ranges::sort(angles);

	for (const double angle : angles)
	{
		const double radius = m_baseSize * GetRandomDouble(0.7, 1.3);

		const double localX = radius * cos(angle);
		const double localY = radius * sin(angle);
		m_localPoints.emplace_back(localX, localY);
	}
}

// Ray casting algorithm
bool PointInPolygon(const double x, const double y, const std::vector<Point>& polygon)
{
	// TODO уточнить, что с делением на ноль
	bool inside = false;

	for (size_t i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++)
	{
		const auto vi = polygon[i];
		const auto vj = polygon[j];

		if (((vi.y > y) != (vj.y > y)) && (x < (vj.x - vi.x) * (y - vi.y) / (vj.y - vi.y) + vi.x))
		{
			inside = !inside;
		}
	}

	return inside;
}

std::vector<Asteroid> CreateTwoAsteroids(
	const Point& pos,
	double baseSize,
	AsteroidSize newSize)
{
	std::vector<Asteroid> result;

	auto& rng = GetRandomGenerator();
	std::uniform_real_distribution angleDist(0.0, 2 * M_PI);

	for (int i = 0; i < 2; i++)
	{
		const double angle = angleDist(rng);

		Vector velocity = {
			cos(angle) * Asteroid::SPEED,
			sin(angle) * Asteroid::SPEED
		};

		result.emplace_back(pos, velocity, newSize, baseSize);
	}

	return result;
}