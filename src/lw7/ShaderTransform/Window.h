#pragma once
#include "../../Common/Shader/Program.h"
#include "../../Common/BaseWindow/BaseWindow.h"
#include "Grid.h"

class Window final : public BaseWindow
{
public:
	Window(int width, int height, const char* title);

protected:
	void Draw(int width, int height) override;

private:
	void UpdateCameraOrbit();

private:
	struct Camera
	{
		float theta = 1.5;
		float phi = 0.2;
		float radius = 19;
	};
	struct Mouse
	{
		double lastX = 0;
		double lastY = 0;
		float sensitivity = 0.007;
		bool isDragging = false;
	};

	Program m_program;
	Grid m_grid;

	Camera m_camera;
	Mouse m_mouse;
};
