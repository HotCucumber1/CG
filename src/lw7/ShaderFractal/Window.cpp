#include "Window.h"

struct BgColor
{
	float r = 0.08;
	float g = 0.08;
	float b = 0.1;
	float a = 1;
};
constexpr BgColor BG_COLOR;

Window::Window(
	const int width,
	const int height,
	const char* title)
	: BaseWindow(width, height, title)
{
	const auto vertexSrc = LoadShaderSource("./data/shaders/vertex.glsl");
	const auto fragmentSrc = LoadShaderSource("./data/shaders/fragment.glsl");

	m_shaderProgram.Create(
		vertexSrc.c_str(),
		fragmentSrc.c_str());

	m_square.Init();
}

void Window::Draw(const int width, const int height)
{
	glViewport(0, 0, width, height);

	glClearColor(BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, BG_COLOR.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	__glewUseProgram(m_shaderProgram);

	glUniform2d(
		glGetUniformLocation(m_shaderProgram, "uOffset"),
		m_offsetX,
		m_offsetY);
	glUniform1d(
		glGetUniformLocation(m_shaderProgram, "uZoom"),
		m_zoom);

	glUniform2f(
		glGetUniformLocation(m_shaderProgram, "uResolution"),
		static_cast<float>(width),
		static_cast<float>(height));
	m_square.Draw();
}

void Window::OnKeyClick(
	const int key,
	int scancode,
	const int action,
	int mods)
{
	constexpr float stepScale = 0.05;
	constexpr float zoomStep = 0.85;
	if (action == GLFW_RELEASE)
	{
		return;
	}
	const auto step = m_zoom * stepScale;

	switch (key)
	{
	case GLFW_KEY_UP:
		m_offsetY += step;
		break;
	case GLFW_KEY_DOWN:
		m_offsetY -= step;
		break;
	case GLFW_KEY_LEFT:
		m_offsetX -= step;
		break;
	case GLFW_KEY_RIGHT:
		m_offsetX += step;
		break;
	case GLFW_KEY_W:
		m_zoom *= zoomStep;
		break;
	case GLFW_KEY_S:
		m_zoom /= zoomStep;
		break;
	default:
		break;
	}
}
