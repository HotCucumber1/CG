#include "Shape.h"

#include <algorithm>
#include <cmath>
#include <iostream>

constexpr int DEPTH = 500;
constexpr float LINE_WIDTH = 1;
constexpr float ALPHA = 0.5;
constexpr float SCALE = 100;
constexpr float EPS = 0.5;
constexpr float EDGE_LENGTH = 2 * SCALE;

constexpr Color trianglesColor = { 1, 0.5, 0.5 };
constexpr Color quadsColor = { 0, 0.5, 0.5 };
constexpr Color edgesColor = { 0, 0, 0 };

template <size_t N>
void DrawEdgesForFaces(
	const std::vector<std::array<int, N>>& faces,
	const std::vector<Vector3d>& vertices,
	Color color,
	float alpha);

std::vector<Vector3d> GenerateVertices();
Shape::FacesData GenerateShape();
std::vector<std::array<int, Shape::TRIANGLE_VERTICES>> FindTriangles(const std::vector<Vector3d>& vertices);
std::vector<std::array<int, Shape::QUAD_VERTICES>> FindQuads(const std::vector<Vector3d>& vertices);

Shape::Shape(
	const int width,
	const int height,
	const std::string& title)
	: BaseWindow(width, height, title.c_str())
{
	m_facesData = GenerateShape();
}

void Shape::Draw(const int width, const int height)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SetDefaultOrtho(width, height, DEPTH);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(width / 2, height / 2, 0);

	glRotatef(m_rotationX, 1, 0, 0);
	glRotatef(m_rotationY, 0, 1, 0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	DrawEdges(edgesColor);

	glDepthMask(GL_FALSE);
	glEnable(GL_CULL_FACE);

	DrawFaces(GL_FRONT);
	DrawFaces(GL_BACK);

	glDisable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
}

void Shape::DrawTriangles(const Color color, const float alpha) const
{
	glColor4f(color.r, color.g, color.b, alpha);
	glBegin(GL_TRIANGLES);
	for (const auto& face : m_facesData.triangleFaces)
	{
		for (int i = 0; i < TRIANGLE_VERTICES; i++)
		{
			auto v = m_facesData.vertices[face[i]];
			glVertex3f(v.GetX(), v.GetY(), v.GetZ());
		}
	}
	glEnd();
}

void Shape::DrawQuads(const Color color, const float alpha) const
{
	glColor4f(color.r, color.g, color.b, alpha);
	glBegin(GL_QUADS);
	for (const auto& face : m_facesData.quadFaces)
	{
		for (int i = 0; i < QUAD_VERTICES; i++)
		{
			auto v = m_facesData.vertices[face[i]];
			glVertex3f(v.GetX(), v.GetY(), v.GetZ());
		}
	}
	glEnd();
}

void Shape::DrawEdges(const Color color, const float alpha) const
{
	DrawEdgesForFaces<TRIANGLE_VERTICES>(m_facesData.triangleFaces, m_facesData.vertices, color, alpha);
	DrawEdgesForFaces<QUAD_VERTICES>(m_facesData.quadFaces, m_facesData.vertices, color, alpha);
}

void Shape::DrawFaces(const GLenum mode) const
{
	glCullFace(mode);
	DrawTriangles(trianglesColor, ALPHA);
	DrawQuads(quadsColor, ALPHA);
}

void Shape::OnCursorPos(const double x, const double y)
{
	if (!m_isMousePressed)
	{
		m_lastMouseX = x;
		m_lastMouseY = y;
		return;
	}
	const double deltaX = x - m_lastMouseX;
	const double deltaY = y - m_lastMouseY;

	m_rotationY += deltaX;
	m_rotationX += deltaY;
	m_lastMouseX = x;
	m_lastMouseY = y;
}

void Shape::OnMouseButton(const int button, const int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			m_isMousePressed = true;
			glfwGetCursorPos(m_window.get(), &m_lastMouseX, &m_lastMouseY);
		}
		else if (action == GLFW_RELEASE)
		{
			m_isMousePressed = false;
		}
	}
}

Shape::FacesData GenerateShape()
{
	const std::vector<Vector3d> vertices = GenerateVertices();

	return {
		vertices,
		FindTriangles(vertices),
		FindQuads(vertices),
	};
}

std::vector<Vector3d> GenerateVertices()
{
	constexpr float a = 1.0 * SCALE;
	constexpr std::array signs = { -1, 1 };
	const float b = (1.0f + std::sqrt(2.0f)) * SCALE;

	std::array base = { a, a, b };
	std::vector<Vector3d> vertices;
	do
	{
		for (const int sign1 : signs)
		{
			for (const int sign2 : signs)
			{
				for (const int sign3 : signs)
				{
					vertices.emplace_back(
						sign1 * base[0],
						sign2 * base[1],
						sign3 * base[2]);
				}
			}
		}
	} while (std::ranges::next_permutation(base).found);

	return vertices;
}

template <size_t N>
void DrawEdgesForFaces(
	const std::vector<std::array<int, N>>& faces,
	const std::vector<Vector3d>& vertices,
	const Color color,
	const float alpha)
{
	glColor4f(color.r, color.g, color.b, alpha);
	glLineWidth(LINE_WIDTH);
	glBegin(GL_LINES);

	for (const auto& face : faces)
	{
		for (int i = 0; i < N; i++)
		{
			const int next = (i + 1) % N;
			glVertex3f(vertices[face[i]].GetX(),
				vertices[face[i]].GetY(),
				vertices[face[i]].GetZ());
			glVertex3f(vertices[face[next]].GetX(),
				vertices[face[next]].GetY(),
				vertices[face[next]].GetZ());
		}
	}
	glEnd();
}

float GetDist(const std::vector<Vector3d>& vertices, const int i, const int j)
{
	const float dx = vertices[i].GetX() - vertices[j].GetX();
	const float dy = vertices[i].GetY() - vertices[j].GetY();
	const float dz = vertices[i].GetZ() - vertices[j].GetZ();
	return std::sqrt(dx * dx + dy * dy + dz * dz);
}

bool CheckWinding(const std::vector<Vector3d>& vertices, const int i1, const int i2, const int i3)
{
	const auto v1 = vertices[i1];
	const auto v2 = vertices[i2];
	const auto v3 = vertices[i3];
	const float ax = v2.GetX() - v1.GetX();
	const float ay = v2.GetY() - v1.GetY();
	const float az = v2.GetZ() - v1.GetZ();
	const float bx = v3.GetX() - v1.GetX();
	const float by = v3.GetY() - v1.GetY();
	const float bz = v3.GetZ() - v1.GetZ();
	const float nx = ay * bz - az * by;
	const float ny = az * bx - ax * bz;
	const float nz = ax * by - ay * bx;
	return (nx * v1.GetX() + ny * v1.GetY() + nz * v1.GetZ()) >= 0;
}

std::vector<std::array<int, Shape::TRIANGLE_VERTICES>> FindTriangles(const std::vector<Vector3d>& vertices)
{
	std::vector<std::array<int, Shape::TRIANGLE_VERTICES>> triangleFaces;

	for (auto i = 0; i < vertices.size(); i++)
	{
		for (auto j = i + 1; j < vertices.size(); j++)
		{
			for (auto k = j + 1; k < vertices.size(); k++)
			{
				if (std::abs(GetDist(vertices, i, j) - EDGE_LENGTH) < EPS
					&& std::abs(GetDist(vertices, j, k) - EDGE_LENGTH) < EPS
					&& std::abs(GetDist(vertices, k, i) - EDGE_LENGTH) < EPS)
				{
					if (CheckWinding(vertices, i, j, k))
					{
						triangleFaces.push_back({ i, j, k });
					}
					else
					{
						triangleFaces.push_back({ i, k, j });
					}
				}
			}
		}
	}
	return triangleFaces;
}

std::vector<std::array<int, Shape::QUAD_VERTICES>> FindQuads(const std::vector<Vector3d>& vertices)
{
	std::vector<std::array<int, Shape::QUAD_VERTICES>> quadFaces;
	const float diagLength = EDGE_LENGTH * std::sqrt(2.0f);

	for (size_t i = 0; i < vertices.size(); i++)
	{
		for (size_t j = i + 1; j < vertices.size(); j++)
		{
			for (size_t k = j + 1; k < vertices.size(); k++)
			{
				for (size_t m = k + 1; m < vertices.size(); m++)
				{
					float d[] = {
						GetDist(vertices, i, j), GetDist(vertices, i, k), GetDist(vertices, i, m),
						GetDist(vertices, j, k), GetDist(vertices, j, m), GetDist(vertices, k, m)
					};

					int edges = 0;
					int diags = 0;
					for (const auto x : d)
					{
						if (std::abs(x - EDGE_LENGTH) < EPS)
						{
							edges++;
						}
						else if (std::abs(x - diagLength) < EPS)
						{
							diags++;
						}
					}

					if (edges == Shape::QUAD_VERTICES
						&& diags == Shape::QUAD_VERTICES / 2)
					{
						const int v[] = {
							static_cast<int>(i),
							static_cast<int>(j),
							static_cast<int>(k),
							static_cast<int>(m),
						};
						std::array<int, 4> face{};
						if (std::abs(GetDist(vertices, v[0], v[1]) - diagLength) < EPS)
						{
							face = { v[0], v[2], v[1], v[3] };
						}
						else if (std::abs(GetDist(vertices, v[0], v[2]) - diagLength) < EPS)
						{
							face = { v[0], v[1], v[2], v[3] };
						}
						else
						{
							face = { v[0], v[1], v[3], v[2] };
						}
						if (!CheckWinding(vertices, face[0], face[1], face[2]))
						{
							std::swap(face[1], face[3]);
						}
						quadFaces.push_back(face);
					}
				}
			}
		}
	}
	return quadFaces;
}
