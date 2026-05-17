#include "Window.h"

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

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	__glewUseProgram(m_shaderProgram);

	const auto resLoc = __glewGetUniformLocation(m_shaderProgram, "uResolution");
	__glewUniform2f(resLoc, static_cast<float>(width), static_cast<float>(height));

	m_square.Draw();
}
