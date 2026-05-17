#include "Window.h"

#include "../../Common/Shader/Program.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

constexpr float LINE_WIDTH = 2.5;
constexpr float WINDOW_SIZE = 3;

struct BgColor
{
	float r = 0.1;
	float g = 0.11;
	float b = 0.15;
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

	m_line.Init();

	glLineWidth(LINE_WIDTH);
	glEnable(GL_LINE_SMOOTH);
}

void Window::Draw(const int width, const int height)
{
	glViewport(0, 0, width, height);

	glClearColor(BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, BG_COLOR.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	__glewUseProgram(m_shaderProgram);

	const auto projection = glm::ortho(
		-WINDOW_SIZE,
		WINDOW_SIZE,
		-WINDOW_SIZE,
		WINDOW_SIZE);

	const auto projLoc = __glewGetUniformLocation(m_shaderProgram, "uProjection");
	__glewUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

	m_line.Draw();
}
