#pragma once

#include "../../Common/BaseWindow/BaseWindow.h"
#include "../../Common/Shader/Program.h"

#include <GL/glew.h>

class EdgeDetectionWindow final : public BaseWindow
{
public:
	EdgeDetectionWindow(int width, int height, const char* title, const char* imagePath);
	~EdgeDetectionWindow() override;

	void Draw(int width, int height) override;

	void OnKeyClick(int key, int scancode, int action, int mods) override;

private:
	bool LoadTexture(const char* path);

	void SetupQuad();

	void CleanUp() const;

	GLuint m_texture = 0;
	Program m_shaderProgram;
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	int m_angle = 0;
	int m_imageWidth = 0;
	int m_imageHeight = 0;
	int m_windowWidth = 0;
	int m_windowHeight = 0;
};
