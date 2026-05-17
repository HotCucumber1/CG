#include "Grid.h"

#include <cmath>
#include <vector>

std::vector<float> GetVertices(int stepsU, int stepsV);
std::vector<unsigned int> GetIndices(int stepsU, int stepsV);

void Grid::ParameterInit(const int stepsU, const int stepsV)
{
	const auto vertices = GetVertices(stepsU, stepsV);
	const auto indices = GetIndices(stepsU, stepsV);

	m_indexCount = static_cast<GLsizei>(indices.size());

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(
		0, 2,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(float),
		static_cast<void*>(nullptr));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Grid::Draw() const
{
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Grid::Init()
{
}

std::vector<float> GetVertices(
	const int stepsU,
	const int stepsV)
{
	std::vector<float> vertices;

	constexpr float MAX_PI = 2 * M_PI;

	for (int i = 0; i <= stepsU; i++)
	{
		const auto u = (static_cast<float>(i) / stepsU) * MAX_PI;
		for (int j = 0; j <= stepsV; j++)
		{
			const auto v = (static_cast<float>(j) / stepsV) * MAX_PI;

			vertices.push_back(u);
			vertices.push_back(v);
		}
	}
	return vertices;
}

std::vector<unsigned int> GetIndices(const int stepsU, const int stepsV)
{
	std::vector<unsigned int> indices;

	const int verticesPerColumn = stepsV + 1;
	for (int i = 0; i < stepsU; i++)
	{
		for (int j = 0; j < stepsV; j++)
		{
			const unsigned int topLeft = i * verticesPerColumn + j;
			const unsigned int bottomLeft = (i + 1) * verticesPerColumn + j;
			const unsigned int topRight = topLeft + 1;
			const unsigned int bottomRight = bottomLeft + 1;

			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);

			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}
	return indices;
}
