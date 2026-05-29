#include "Window.h"
#include "../../../Common/Shader/Shader.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

constexpr int CAMERA_START_X = 0;
constexpr int CAMERA_START_Y = 0;
constexpr int CAMERA_START_Z = 140;

constexpr float PARTICLE_RADIUS = 7;
constexpr float PARTICLE_MASS = 2;
constexpr float PARTICLE_SCALE = 0.2;

constexpr int NUM_PART = 10;
constexpr float START_G = 0.05;
constexpr int TIME_SCALE = 1;

Window::Window(
	const int width,
	const int height,
	const char* title)
	: BaseWindow(width, height, title)
	, m_particleSystem(NUM_PART, START_G, TIME_SCALE)
{
	m_particleSystem.InitializeRandom(PARTICLE_RADIUS, PARTICLE_MASS, PARTICLE_SCALE);
	m_camera.SetPosition(glm::vec3(CAMERA_START_X, CAMERA_START_Y, CAMERA_START_Z));

	InitShaders();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_PROGRAM_POINT_SIZE);
}

Window::~Window()
{
	__glewDeleteProgram(m_shaderProgram);
}

void Window::InitShaders()
{
	const auto vertexShader = LoadShaderSource("./data/shaders/vertex.glsl");
	const auto fragmentShader = LoadShaderSource("./data/shaders/fragment.glsl");

	m_shaderProgram.Create(vertexShader.c_str(), fragmentShader.c_str());
	__glewUseProgram(m_shaderProgram);

	m_modelViewProjectionUniform = __glewGetUniformLocation(m_shaderProgram, "modelViewProjection");
	m_pointSizeUniform = __glewGetUniformLocation(m_shaderProgram, "pointSize");
	m_particleColorUniform = __glewGetUniformLocation(m_shaderProgram, "particleColor");

	__glewUniform1f(m_pointSizeUniform, 8.0f);
	__glewUniform3f(m_particleColorUniform, 1.0f, 0.6f, 0.2f);
}

void Window::Draw(const int width, const int height)
{
	static auto lastTime = glfwGetTime();
	const auto currentTime = glfwGetTime();
	const auto deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	m_particleSystem.Update(deltaTime);

	glViewport(0, 0, width, height);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const auto aspectRatio = static_cast<float>(width) / height;
	const auto projection = m_camera.GetProjectionMatrix(aspectRatio);
	const auto view = m_camera.GetViewMatrix();
	constexpr auto model = glm::mat4(1);
	auto mvp = projection * view * model;

	__glewUseProgram(m_shaderProgram);
	__glewUniformMatrix4fv(m_modelViewProjectionUniform, 1, GL_FALSE, glm::value_ptr(mvp));

	m_particleSystem.Render();

	const auto info = "Particles: " + std::to_string(m_particleSystem.GetParticleCount())
		+ " | G: " + std::to_string(m_G)
		+ " | Speed: " + std::to_string(m_timeScale) + "x";
	glfwSetWindowTitle(m_window.get(), info.c_str());
}

void Window::OnKeyClick(
	const int key,
	int scancode,
	const int action,
	const int mods)
{
	static constexpr float gStep = 0.05;
	static constexpr float timeScale = 0.1;

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		m_camera.ProcessKeyboard(key, 1 / 60.0f);

		switch (key)
		{
		case GLFW_KEY_UP:
			m_timeScale += timeScale;
			m_particleSystem.SetTimeScale(m_timeScale);
			break;
		case GLFW_KEY_DOWN:
			m_timeScale -= timeScale;
			if (m_timeScale < 0)
			{
				m_timeScale = 0;
			}
			m_particleSystem.SetTimeScale(m_timeScale);
			break;
		case GLFW_KEY_RIGHT:
			m_G += gStep;
			m_particleSystem.SetGravitationalConstant(m_G);
			break;
		case GLFW_KEY_LEFT:
			m_G -= gStep;
			if (m_G < 0)
			{
				m_G = 0;
			}
			m_particleSystem.SetGravitationalConstant(m_G);
			break;
		case GLFW_KEY_SPACE:
			m_particleSystem.InitializeRandom(PARTICLE_RADIUS, PARTICLE_MASS, PARTICLE_SCALE);
			break;
		case GLFW_KEY_R:
			m_camera.SetPosition(glm::vec3(0, 0, 40));
			break;
		default:
			break;
		}
	}
}

void Window::OnCursorPos(
	const double x,
	const double y)
{
	if (m_mousePressed)
	{
		const auto xOffset = x - m_lastMouseX;
		const auto yOffset = m_lastMouseY - y;
		m_camera.ProcessMouseMovement(xOffset, yOffset);
	}
	m_lastMouseX = x;
	m_lastMouseY = y;
}

void Window::OnMouseButton(const int button, const int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			m_mousePressed = true;
			glfwSetInputMode(m_window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else if (action == GLFW_RELEASE)
		{
			m_mousePressed = false;
			glfwSetInputMode(m_window.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}
