#pragma once
#include "../../Common/Shader/Program.h"
#include "../../Common/BaseWindow/BaseWindow.h"
#include "SquareMesh.h"

class Window final : public BaseWindow
{
public:
	Window(int width, int height, const char* title);

protected:
	void Draw(int width, int height) override;

	void OnKeyClick(int key, int scancode, int action, int mods) override;

private:
	void UpdateCameraOrbit();

private:
	double m_offsetX = -0.5;
	double m_offsetY = 0;
	double m_zoom = 3;

	Program m_shaderProgram;
	SquareMesh m_square;
};
