#include "../EdgeDetectionWindow.h"
#include <cmath>
#include <iostream>
#include <stb_image.h>

EdgeDetectionWindow::EdgeDetectionWindow(int width, int height, const char* title, const char* imagePath)
	: BaseWindow(width, height, title)
	, m_windowWidth(width)
	, m_windowHeight(height)
{
	if (!LoadTexture(imagePath))
	{
		throw std::runtime_error("Failed to load image");
	}

	const auto vertexSrc = LoadShaderSource("./data/shaders/vertex.glsl");
	const auto fragmentSrc = LoadShaderSource("./data/shaders/fragment.glsl");

	m_shaderProgram.Create(vertexSrc.c_str(), fragmentSrc.c_str());
	SetupQuad();

	const std::string titleText = std::string(title) + " | Angle: " + std::to_string(m_angle) + "°";
	glfwSetWindowTitle(m_window.get(), titleText.c_str());
}

EdgeDetectionWindow::~EdgeDetectionWindow()
{
	CleanUp();
}

void EdgeDetectionWindow::Draw(const int width, const int height)
{
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	__glewUseProgram(m_shaderProgram);

	__glewUniform1f(__glewGetUniformLocation(m_shaderProgram, "uAngleRad"), m_angle * M_PI / 180.0f);
	__glewUniform2f(__glewGetUniformLocation(m_shaderProgram, "uTexelSize"), 1.0f / m_imageWidth, 1.0f / m_imageHeight);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	__glewUniform1i(__glewGetUniformLocation(m_shaderProgram, "uTexture"), 0);

	__glewBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	__glewBindVertexArray(0);

	__glewUseProgram(0);
}

void EdgeDetectionWindow::OnKeyClick(int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS && action != GLFW_REPEAT)
	{
		return;
	}

	if (key == GLFW_KEY_LEFT)
	{
		m_angle = (m_angle - 1 + 360) % 360;
		std::string title = "Edge Detection | Angle: " + std::to_string(m_angle) + "°";
		glfwSetWindowTitle(m_window.get(), title.c_str());
	}
	else if (key == GLFW_KEY_RIGHT)
	{
		m_angle = (m_angle + 1) % 360;
		std::string title = "Edge Detection | Angle: " + std::to_string(m_angle) + "°";
		glfwSetWindowTitle(m_window.get(), title.c_str());
	}
	else if (key == GLFW_KEY_ESCAPE)
	{
		glfwSetWindowShouldClose(m_window.get(), GLFW_TRUE);
	}
}

bool EdgeDetectionWindow::LoadTexture(const char* path)
{
	stbi_set_flip_vertically_on_load(true);
	const auto data = stbi_load(path, &m_imageWidth, &m_imageHeight, nullptr, STBI_rgb);
	if (!data)
	{
		std::cerr << "Failed to load image: " << path << std::endl;
		return false;
	}

	std::cout << "Image loaded: " << m_imageWidth << "x" << m_imageHeight << std::endl;
	std::cout << "Window size: " << m_windowWidth << "x" << m_windowHeight << std::endl;

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_imageWidth, m_imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	__glewGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);
	return true;
}

void EdgeDetectionWindow::SetupQuad()
{
	const float windowAspect = static_cast<float>(m_windowWidth) / m_windowHeight;
	const float imageAspect = static_cast<float>(m_imageWidth) / m_imageHeight;

	float quadWidth;
	float quadHeight;
	if (imageAspect > windowAspect)
	{
		quadWidth = 1.0f;
		quadHeight = windowAspect / imageAspect;
	}
	else
	{
		quadWidth = imageAspect / windowAspect;
		quadHeight = 1.0f;
	}

	const float vertices[] = {
		-quadWidth, quadHeight, 0.0f, 1.0f,
		-quadWidth, -quadHeight, 0.0f, 0.0f,
		quadWidth, quadHeight, 1.0f, 1.0f,
		quadWidth, -quadHeight, 1.0f, 0.0f
	};

	__glewGenVertexArrays(1, &m_vao);
	__glewGenBuffers(1, &m_vbo);

	__glewBindVertexArray(m_vao);
	__glewBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	__glewBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	__glewVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), static_cast<void*>(nullptr));
	__glewEnableVertexAttribArray(0);

	__glewVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
	__glewEnableVertexAttribArray(1);

	__glewBindBuffer(GL_ARRAY_BUFFER, 0);
	__glewBindVertexArray(0);
}

void EdgeDetectionWindow::CleanUp() const
{
	if (m_texture)
	{
		glDeleteTextures(1, &m_texture);
	}
	if (m_vao)
	{
		__glewDeleteVertexArrays(1, &m_vao);
	}
	if (m_vbo)
	{
		__glewDeleteBuffers(1, &m_vbo);
	}
}