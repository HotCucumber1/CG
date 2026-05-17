#include "LineMash.h"

#include <cmath>
#include <vector>

LineMash::~LineMash()
{
	CleanUp();
}

void LineMash::Init()
{
	std::vector<float> vertices;
	constexpr int steps = 2000;
	constexpr float stepSize = (2 * M_PI) / steps;

	for (int i = 0; i <= steps; i++)
	{
		const auto x = i * stepSize;
		vertices.push_back(x);
		vertices.push_back(0);
		vertices.push_back(0);
	}

	m_vertexCount = static_cast<GLsizei>(vertices.size() / 3);
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),
		vertices.data(),
		GL_STATIC_DRAW);

	glVertexAttribPointer(
		0, 3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(float),
		static_cast<void*>(nullptr));

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void LineMash::Draw() const
{
	glBindVertexArray(m_vao);
	glDrawArrays(GL_LINE_STRIP, 0, m_vertexCount);
	glBindVertexArray(0);
}

void LineMash::CleanUp() const
{
	if (m_vbo)
	{
		glDeleteBuffers(1, &m_vbo);
	}
	if (m_vao)
	{
		glDeleteVertexArrays(1, &m_vao);
	}
}
