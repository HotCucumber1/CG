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

private:
	Program m_shaderProgram;
	SquareMesh m_square;
};
