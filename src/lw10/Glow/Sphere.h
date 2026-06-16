#pragma once
#include <glm/vec3.hpp>
#include <cmath>
#include <vector>

struct Sphere
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
};

inline Sphere GetSphere()
{
	Sphere sphere;

	constexpr unsigned int X_SEGMENTS = 64;
	constexpr unsigned int Y_SEGMENTS = 64;

	for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
	{
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
		{
			const auto xSegment = static_cast<float>(x) / X_SEGMENTS;
			const auto ySegment = static_cast<float>(y) / Y_SEGMENTS;
			const auto xPos = std::cos(xSegment * 2 * M_PI) * std::sin(ySegment * M_PI);
			const auto yPos = std::cos(ySegment * M_PI);
			const auto zPos = std::sin(xSegment * 2 * M_PI) * std::sin(ySegment * M_PI);

			sphere.positions.emplace_back(xPos, yPos, zPos);
			sphere.normals.emplace_back(xPos, yPos, zPos);
		}
	}

	bool oddRow = false;
	for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
	{
		if (!oddRow)
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				sphere.indices.push_back(y * (X_SEGMENTS + 1) + x);
				sphere.indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS; x >= 0; --x)
			{
				sphere.indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				sphere.indices.push_back(y * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}
	return sphere;
}
