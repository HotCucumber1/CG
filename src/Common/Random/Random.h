#pragma once
#include <ctime>
#include <random>

inline std::mt19937& GetRandomGenerator()
{
	static std::mt19937 rng(static_cast<unsigned>(time(nullptr)));
	return rng;
}

inline double GetRandomDouble(const double min, const double max)
{
	std::uniform_real_distribution dist(min, max);
	return dist(GetRandomGenerator());
}

inline int GetRandomInt(const int min, const int max)
{
	std::uniform_int_distribution dist(min, max);
	return dist(GetRandomGenerator());
}

/**
 * Return random angle from 0 to 360 degree
 */
inline int GetRandomAngle()
{
	std::uniform_int_distribution dist(0, 360);
	return dist(GetRandomGenerator());
}
