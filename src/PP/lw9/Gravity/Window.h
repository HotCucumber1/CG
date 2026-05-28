#pragma once
#include "../../../Common/Shader/Program.h"
#include "../../../Common/BaseWindow/BaseWindow.h"
#include "Camera.h"
#include "ParticleSystem.h"

class Window final : public BaseWindow
{
public:
	Window(int width, int height, const char* title);
	~Window() override;

private:
	void Draw(int width, int height) override;

	void OnKeyClick(int key, int scancode, int action, int mods) override;

	void OnCursorPos(double x, double y) override;

	void OnMouseButton(int button, int action, int mods) override;

	void InitShaders();

private:
	Program m_shaderProgram;
	ParticleSystem m_particleSystem;
	Camera m_camera;

	GLint m_modelViewProjectionUniform{};
	GLint m_pointSizeUniform{};
	GLint m_particleColorUniform{};

	bool m_mousePressed = false;
	double m_lastMouseX = 0;
	double m_lastMouseY = 0;

	float m_G = 0.05;
	float m_timeScale = 1;

	float m_rotationAngle = 0;
	glm::mat4 m_modelMatrix{};
};
