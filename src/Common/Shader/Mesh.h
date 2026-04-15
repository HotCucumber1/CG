#pragma once
#include "../Component/Vector.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <utility>
#include <vector>

struct Vertex
{
	Vector3f position;
	Vector3f normal;
	Vector2f texCoords;

	Vertex() = default;
	Vertex(const Vector3f& pos, const Vector3f& norm, const Vector2f& tex)
		: position(pos)
		, normal(norm)
		, texCoords(tex)
	{
	}
};

class Mesh
{
public:
	Mesh() = default;

	Mesh(
		const std::vector<Vertex>& vertices,
		const std::vector<unsigned int>& indices)
		: Mesh()
	{
		SetData(vertices, indices);
	}

	~Mesh()
	{
		Cleanup();
	}

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	Mesh(Mesh&& other) noexcept
		: m_vao(std::exchange(other.m_vao, 0))
		, m_vbo(std::exchange(other.m_vbo, 0))
		, m_ebo(std::exchange(other.m_ebo, 0))
		, m_indexCount(std::exchange(other.m_indexCount, 0))
		, m_vertexCount(std::exchange(other.m_vertexCount, 0))
	{
	}

	Mesh& operator=(Mesh&& other) noexcept
	{
		if (this != &other)
		{
			Cleanup();
			m_vao = std::exchange(other.m_vao, 0);
			m_vbo = std::exchange(other.m_vbo, 0);
			m_ebo = std::exchange(other.m_ebo, 0);
			m_vertexCount = std::exchange(other.m_vertexCount, 0);
			m_indexCount = std::exchange(other.m_indexCount, 0);
		}
		return *this;
	}

	void SetData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	{
		Cleanup();

		m_vertexCount = vertices.size();
		m_indexCount = indices.size();

		if (m_vertexCount == 0)
		{
			return;
		}

		// TODO это что
		__glewGenVertexArrays(1, &m_vao);
		__glewBindVertexArray(m_vao);

		__glewGenBuffers(1, &m_vbo);
		__glewBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		__glewBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
		if (m_indexCount > 0)
		{
			__glewGenBuffers(1, &m_ebo);
			__glewBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
			__glewBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
		}

		SetupMesh();
		__glewBindVertexArray(0);
	}

	void Draw() const
	{
		DrawInstanced(1);
	}

	void DrawInstanced(const int instanceCount) const
	{
		if (!IsValid() || instanceCount <= 0)
		{
			return;
		}

		__glewBindVertexArray(m_vao);
		if (m_indexCount > 0)
		{
			__glewDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(m_indexCount), GL_UNSIGNED_INT, nullptr, instanceCount);
		}
		else
		{
			__glewDrawArraysInstanced(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertexCount), instanceCount);
		}
		__glewBindVertexArray(0);
	}

	bool IsValid() const
	{
		return m_vao != 0;
	}

	size_t GetVertexCount() const
	{
		return m_vertexCount;
	}

	size_t GetIndexCount() const
	{
		return m_indexCount;
	}

private:
	void Cleanup()
	{
		if (m_ebo != 0)
		{
			__glewDeleteBuffers(1, &m_ebo);
			m_ebo = 0;
		}
		if (m_vbo != 0)
		{
			__glewDeleteBuffers(1, &m_vbo);
			m_vbo = 0;
		}
		if (m_vao != 0)
		{
			__glewDeleteVertexArrays(1, &m_vao);
			m_vao = 0;
		}
		m_vertexCount = 0;
		m_indexCount = 0;
	}

	static void SetupMesh()
	{
		__glewEnableVertexAttribArray(0);
		__glewVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));

		__glewEnableVertexAttribArray(1);
		__glewVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

		__glewEnableVertexAttribArray(2);
		__glewVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords)));
	}

private:
	GLuint m_vao = 0; // TODO че это
	GLuint m_vbo = 0;
	GLuint m_ebo = 0;
	size_t m_indexCount = 0;
	size_t m_vertexCount = 0;
};

inline Mesh CreateCubeMesh()
{
	// TODO исправить
	std::vector<Vertex> vertices = {
		// Передняя грань (z = 0.5)
		{ { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
		{ { 0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
		{ { 0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
		{ { -0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },

		// Задняя грань (z = -0.5)
		{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f } },
		{ { 0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
		{ { 0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } },
		{ { -0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } },

		// Левая грань (x = -0.5)
		{ { -0.5f, -0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
		{ { -0.5f, -0.5f, 0.5f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
		{ { -0.5f, 0.5f, 0.5f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
		{ { -0.5f, 0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },

		// Правая грань (x = 0.5)
		{ { 0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
		{ { 0.5f, -0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
		{ { 0.5f, 0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
		{ { 0.5f, 0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },

		// Верхняя грань (y = 0.5)
		{ { -0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
		{ { 0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
		{ { 0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
		{ { -0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },

		// Нижняя грань (y = -0.5)
		{ { -0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
		{ { 0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } },
		{ { 0.5f, -0.5f, 0.5f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } },
		{ { -0.5f, -0.5f, 0.5f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } }
	};

	std::vector<unsigned int> indices = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20
	};

	return { vertices, indices };
}
inline Mesh CreateSphereMesh(
	const float radius,
	const int segments,
	const int rings)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (int ring = 0; ring <= rings; ++ring)
	{
		const auto phi = static_cast<float>(ring) * static_cast<float>(M_PI) / static_cast<float>(rings);
		const auto y = std::cos(phi);
		const auto ringRadius = std::sin(phi);

		for (int seg = 0; seg <= segments; ++seg)
		{
			const auto theta = static_cast<float>(seg) * 2 * static_cast<float>(M_PI) / static_cast<float>(segments);
			const auto x = ringRadius * std::cos(theta);
			const auto z = ringRadius * std::sin(theta);

			Vector3f position(x * radius, y * radius, z * radius);
			Vector3f normal(x, y, z);
			Vector2f texCoords(
				static_cast<float>(seg) / static_cast<float>(segments),
				static_cast<float>(ring) / static_cast<float>(rings));
			vertices.emplace_back(position, normal, texCoords);
		}
	}

	for (int ring = 0; ring < rings; ++ring)
	{
		for (int seg = 0; seg < segments; ++seg)
		{
			const auto current = ring * (segments + 1) + seg;
			const auto next = current + 1;
			const auto bottom = (ring + 1) * (segments + 1) + seg;
			const auto bottomNext = bottom + 1;

			indices.push_back(current);
			indices.push_back(bottom);
			indices.push_back(next);

			indices.push_back(next);
			indices.push_back(bottom);
			indices.push_back(bottomNext);
		}
	}

	return { vertices, indices };
}

inline Mesh CreatePlaneMesh(const float width, const float height)
{
	const auto halfW = width / 2;
	const auto halfH = height / 2;

	std::vector<Vertex> vertices = {
		{ { -halfW, 0.0f, -halfH }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
		{ { halfW, 0.0f, -halfH }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
		{ { halfW, 0.0f, halfH }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
		{ { -halfW, 0.0f, halfH }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } }
	};

	std::vector<unsigned int> indices = {
		0, 1, 2,
		2, 3, 0
	};

	return { vertices, indices };
}
