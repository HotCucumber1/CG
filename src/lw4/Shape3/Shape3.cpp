#include "Shape3.h"

#include <algorithm>
#include <iostream>

constexpr int DEPTH = 500;
constexpr int U_STEPS = 60;
constexpr int V_STEPS = 10;
constexpr float SCALE = 200;
constexpr float LINE_WIDTH = 1;

constexpr Color surfaceColor = { 0.2, 0.6, 1 };
constexpr Color edgesColor = { 0, 0, 0 };

Color CalculateColor(const Vector3d& v);

Shape3::Shape3(
	const int width,
	const int height,
	const std::string& title)
	: BaseWindow(
		  width,
		  height,
		  title.c_str())
{
}

void Shape3::Draw(const int width, const int height)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SetDefaultOrtho(width, height, DEPTH);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(width / 2, height / 2, 0);

	glRotatef(m_rotationX, 1, 0, 0);
	glRotatef(m_rotationY, 0, 1, 0);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glShadeModel(GL_SMOOTH);
	DrawQuads(1);

	glDisable(GL_DEPTH_TEST);
}

void Shape3::DrawQuads(const float alpha) const
{
	glBegin(GL_QUADS);
	for (const auto& face : m_facesData.GetQuadFaces())
	{
		for (int i = 0; i < MobiusStrip::QUAD_VERTICES; i++)
		{
			auto v = m_facesData.GetVertices()[face[i]];
			const auto [r, g, b] = CalculateColor(v);

			glColor4f(r, g, b, alpha);
			glVertex3f(v.GetX(), v.GetY(), v.GetZ());
		}
	}
	glEnd();
}

void Shape3::DrawEdges(const Color color) const
{
	glColor3f(color.r, color.g, color.b);
	glLineWidth(LINE_WIDTH);
	glBegin(GL_LINES);

	for (const auto& face : m_facesData.GetQuadFaces())
	{
		for (int i = 0; i < MobiusStrip::QUAD_VERTICES; i++)
		{
			const int next = (i + 1) % MobiusStrip::QUAD_VERTICES;
			glVertex3f(m_facesData.GetVertices()[face[i]].GetX(), m_facesData.GetVertices()[face[i]].GetY(), m_facesData.GetVertices()[face[i]].GetZ());
			glVertex3f(m_facesData.GetVertices()[face[next]].GetX(), m_facesData.GetVertices()[face[next]].GetY(), m_facesData.GetVertices()[face[next]].GetZ());
		}
	}
	glEnd();
}

void Shape3::OnCursorPos(
	const double x,
	const double y)
{
	if (!m_isMousePressed)
	{
		m_lastMouseX = x;
		m_lastMouseY = y;
		return;
	}
	const auto deltaX = x - m_lastMouseX;
	const auto deltaY = y - m_lastMouseY;

	m_rotationY += deltaX;
	m_rotationX += deltaY;
	m_lastMouseX = x;
	m_lastMouseY = y;
}

void Shape3::OnMouseButton(const int button, const int action, int mods)
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

Color CalculateColor(const Vector3d& v)
{
	float zNormalized = (v.GetZ() + SCALE / 2) / SCALE;
	zNormalized = std::clamp(zNormalized, 0.0f, 1.0f);

	const float r = 0.5 + (0.5 * zNormalized);
	const float g = zNormalized;
	const float b = 0.5 + (0.5 * zNormalized);
	return { r, g, b };
}
