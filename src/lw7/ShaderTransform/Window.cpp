#include "Window.h"

#include <cmath>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

constexpr int BASE_STEP = 150;
constexpr float MORPH_SPEED = 0.4;
constexpr float MORPH_SCALE = 0.5;
constexpr float PHI_MAX = 1.5;

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

	m_program.Create(
		vertexSrc.c_str(),
		fragmentSrc.c_str());

	m_grid.ParameterInit(BASE_STEP, BASE_STEP);
	glEnable(GL_DEPTH_TEST);
}

void Window::Draw(const int width, const int height)
{
	glViewport(0, 0, width, height);
	glClearColor(BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, BG_COLOR.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	UpdateCameraOrbit();

	const glm::vec3 cameraPos{
		m_camera.radius * std::cos(m_camera.phi) * std::sin(m_camera.theta),
		m_camera.radius * std::sin(m_camera.phi),
		m_camera.radius * std::cos(m_camera.phi) * std::cos(m_camera.theta),
	};

	const auto view = glm::lookAt(
		cameraPos,
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));
	const auto projection = glm::perspective(
		glm::radians(45.f),
		static_cast<float>(width) / height,
		0.1f, 100.f);
	constexpr auto model = glm::mat4(1);

	const auto time = static_cast<float>(glfwGetTime());
	const float morphFactor = std::sin(MORPH_SPEED * time) * MORPH_SCALE + 0.5;

	__glewUseProgram(m_program);

	__glewUniformMatrix4fv(__glewGetUniformLocation(m_program, "uModel"), 1, GL_FALSE, &model[0][0]);
	__glewUniformMatrix4fv(__glewGetUniformLocation(m_program, "uView"), 1, GL_FALSE, &view[0][0]);
	__glewUniformMatrix4fv(__glewGetUniformLocation(m_program, "uProjection"), 1, GL_FALSE, &projection[0][0]);
	__glewUniform1f(__glewGetUniformLocation(m_program, "uMorphFactor"), morphFactor);

	__glewUniform3f(__glewGetUniformLocation(m_program, "uLightPos"), 5, 8, 6);
	__glewUniform3f(__glewGetUniformLocation(m_program, "uViewPos"), cameraPos.x, cameraPos.y, cameraPos.z);

	m_grid.Draw();
}

void Window::UpdateCameraOrbit()
{
	const auto nativeWin = glfwGetCurrentContext();

	if (glfwGetMouseButton(nativeWin, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		double mouseX, mouseY;
		glfwGetCursorPos(nativeWin, &mouseX, &mouseY);

		if (!m_mouse.isDragging)
		{
			m_mouse.lastX = mouseX;
			m_mouse.lastY = mouseY;
			m_mouse.isDragging = true;
		}

		m_camera.theta -= static_cast<float>(mouseX - m_mouse.lastX) * m_mouse.sensitivity;
		m_camera.phi += static_cast<float>(mouseY - m_mouse.lastY) * m_mouse.sensitivity;

		if (m_camera.phi > PHI_MAX)
		{
			m_camera.phi = PHI_MAX;
		}
		if (m_camera.phi < -PHI_MAX)
		{
			m_camera.phi = -PHI_MAX;
		}

		m_mouse.lastX = mouseX;
		m_mouse.lastY = mouseY;
	}
	else
	{
		m_mouse.isDragging = false;
	}
}
