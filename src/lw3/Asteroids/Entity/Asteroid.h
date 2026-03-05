#pragma once
#include "../../../Common/Component/BaseComponent.h"

#include <vector>

enum class AsteroidSize
{
	Small,
	Medium,
	Large,
};

class Asteroid
{
public:
	static constexpr int BASE_SIZE = 50;
	static constexpr double SPEED = 0.05;

	Asteroid(
		Point center,
		Vector velocity,
		AsteroidSize type,
		double baseSize);

	void Update();

	void BoundPosition(int width, int height);

	Point GetPosition() const;

	std::vector<Point> GetLocalPoints() const;

	AsteroidSize GetSize() const;

	bool IsPointInside(const Point& point) const;

	std::vector<Asteroid> Split() const;

private:
	void GenerateShape();

private:
	Point m_pos;
	Vector m_velocity;
	double m_baseSize;
	AsteroidSize m_size;
	std::vector<Point> m_localPoints;
};
