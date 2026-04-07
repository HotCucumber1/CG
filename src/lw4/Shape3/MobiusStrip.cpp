#include "MobiusStrip.h"
#include <cmath>

MobiusStrip::MobiusStrip()
{
	Generate();
}

std::vector<Vector3d> MobiusStrip::GetVertices() const
{
	return m_vertices;
}

std::vector<std::array<int, MobiusStrip::QUAD_VERTICES>> MobiusStrip::GetQuadFaces() const
{
	return m_quadFaces;
}

void MobiusStrip::Generate()
{
	for (int i = 0; i <= U_STEPS; i++)
	{
		const auto u = static_cast<float>(i) / U_STEPS * 2 * std::numbers::pi_v<float>;

		for (int j = 0; j <= V_STEPS; j++)
		{
			const auto v = -1 + 2 * static_cast<float>(j) / V_STEPS;

			const auto x = (1 + (v / 2) * std::cos(u / 2)) * std::cos(u) * SCALE;
			const auto y = (1 + (v / 2) * std::cos(u / 2)) * std::sin(u) * SCALE;
			const auto z = (v / 2) * std::sin(u / 2) * SCALE;

			m_vertices.emplace_back(x, y, z);
		}
	}

	for (int i = 0; i < U_STEPS; i++)
	{
		for (int j = 0; j < V_STEPS; j++)
		{
			const auto p1 = i * (V_STEPS + 1) + j;
			const auto p2 = p1 + 1;
			const auto p3 = (i + 1) * (V_STEPS + 1) + j + 1;
			const auto p4 = (i + 1) * (V_STEPS + 1) + j;
			m_quadFaces.push_back({ p1, p2, p3, p4 });
		}
	}
}