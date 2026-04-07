#pragma once
#include "../../Common/Component/Vector3.h"

#include <vector>
#include <array>

class MobiusStrip
{
public:
	static constexpr int QUAD_VERTICES = 4;
	static constexpr int U_STEPS = 60;
	static constexpr int V_STEPS = 10;
	static constexpr float SCALE = 200;

	MobiusStrip();

	std::vector<Vector3d> GetVertices() const;

	std::vector<std::array<int, QUAD_VERTICES>> GetQuadFaces() const;

private:
	void Generate();

private:
	std::vector<Vector3d> m_vertices;
	std::vector<std::array<int, QUAD_VERTICES>> m_quadFaces;
};
