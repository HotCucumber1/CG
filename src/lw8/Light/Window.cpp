#include "Window.h"

#include <iostream>

Window::Window(const int width, const int height, const char* title)
	: BaseWindow(width, height, title)
	, m_rayTracer(width, height)
	, m_winWidth(width)
	, m_winHeight(height)
{
	const auto vertexShader = LoadShaderSource("./data/shaders/vertex.glsl");
	const auto fragmentShader = LoadShaderSource("./data/shaders/fragment.glsl");
	m_program.Create(
		vertexShader.c_str(),
		fragmentShader.c_str());

	glUseProgram(m_program);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	m_pixels.resize(width * height * 3);

	constexpr float vertices[] = {
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};
	const unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_vbo);
	glBindVertexArray(m_vao);

	GLuint vbo;
	GLuint ebo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), static_cast<void*>(nullptr));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	InitScene();
}

Window::~Window()
{
	glDeleteTextures(1, &m_texture);
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}

void Window::InitScene()
{
	const Light radiusLight(
		Vector3(0.0f, 4.0f, -1.0f),
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),
		Color(0.2f, 0.2f, 0.2f),
		m_lightRadius);
	m_rayTracer.AddLight(radiusLight);

	const Material redMaterial(
		Color(0.8f, 0.2f, 0.2f),
		Color(1.0f, 1.0f, 1.0f),
		Color(0.2f, 0.05f, 0.05f),
		32.0f);

	const Material blueMaterial(
		Color(0.2f, 0.3f, 0.8f),
		Color(1.0f, 1.0f, 1.0f),
		Color(0.05f, 0.05f, 0.2f),
		64.0f);

	const Material greenMaterial(
		Color(0.2f, 0.8f, 0.2f),
		Color(0.8f, 0.8f, 0.8f),
		Color(0.05f, 0.2f, 0.05f),
		16.0f);

	m_rayTracer.AddSphere(Sphere(Vector3(-1.2f, 0.0f, 0.0f), 0.8f, redMaterial));
	m_rayTracer.AddSphere(Sphere(Vector3(1.2f, 0.0f, 0.0f), 0.8f, blueMaterial));
	m_rayTracer.AddSphere(Sphere(Vector3(0.0f, -1.0f, 0.5f), 0.6f, greenMaterial));

	const Material floorMaterial(
		Color(0.5f, 0.5f, 0.5f),
		Color(0.1f, 0.1f, 0.1f),
		Color(0.2f, 0.2f, 0.2f),
		1.0f);
	m_rayTracer.AddSphere(
		Sphere(Vector3(0.0f, -200.0f, 0.0f), 190.0f, floorMaterial));

	m_rayTracer.SetCamera(
		Vector3(0, 3, 4),
		Vector3(0, 0, 0),
		Vector3(0, 1, 0),
		60);
	m_rayTracer.SetBackground(Color(1, 1, 1));

	m_rayTracer.Render(m_pixels);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_winWidth, m_winHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_pixels.data());
}

void Window::UpdateTexture() const
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_winWidth, m_winHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_pixels.data());
}

void Window::UpdateRayTracerSettings()
{
	m_rayTracer.SetSoftShadowsEnabled(m_softShadows);
	m_rayTracer.SetLightSamplesCount(m_samples);

	m_rayTracer.ClearLights();
	const Light light(
		Vector3(3.0f, 4.0f, 2.0f),
		Color(1.0f, 1.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f),
		Color(0.2f, 0.2f, 0.2f),
		m_lightRadius);
	m_rayTracer.AddLight(light);

	m_needsUpdate = true;
}

void Window::Draw(const int width, const int height)
{
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_needsUpdate)
	{
		m_rayTracer.Render(m_pixels);
		UpdateTexture();
		m_needsUpdate = false;
	}

	glUseProgram(m_program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i(glGetUniformLocation(m_program, "uTexture"), 0);

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Window::OnKeyClick(const int key, int scancode, const int action, int mods)
{
	if (action != GLFW_PRESS)
	{
		return;
	}

	switch (key)
	{
	case GLFW_KEY_S:
		m_softShadows = !m_softShadows;
		std::cout << "Soft shadows: " << (m_softShadows ? "ON" : "OFF") << std::endl;
		UpdateRayTracerSettings();
		break;
	case GLFW_KEY_UP:
		m_samples = std::min(256, m_samples * 2);
		std::cout << "Samples: " << m_samples << std::endl;
		UpdateRayTracerSettings();
		break;
	case GLFW_KEY_DOWN:
		m_samples = std::max(1, m_samples / 2);
		std::cout << "Samples: " << m_samples << std::endl;
		UpdateRayTracerSettings();
		break;
	case GLFW_KEY_RIGHT:
		m_lightRadius += 0.1;
		std::cout << "Light radius: " << m_lightRadius << std::endl;
		UpdateRayTracerSettings();
		break;
	case GLFW_KEY_LEFT:
		m_lightRadius = std::max(0.f, m_lightRadius - 0.1f);
		std::cout << "Light radius: " << m_lightRadius << std::endl;
		UpdateRayTracerSettings();
		break;
	case GLFW_KEY_R:
		m_needsUpdate = true;
		break;
	default:
		break;
	}
}
