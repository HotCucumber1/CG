#pragma once
#include "../../Common/Shader/Program.h"
#include "../../Common/BaseWindow/BaseWindow.h"
#include "RayTracer.h"

class Window final : public BaseWindow
{
public:
	Window(int width, int height, const char* title);

	~Window() override;

	void OnKeyClick(int key, int scancode, int action, int mods) override;

private:
	void Draw(int width, int height) override;

	void InitScene();

	void UpdateTexture() const;

	void UpdateRayTracerSettings();

	RayTracer m_rayTracer;
	Program m_program;
	unsigned int m_texture{};
	unsigned int m_vao{};
	unsigned int m_vbo{};
	std::vector<unsigned char> m_pixels;
	int m_winWidth;
	int m_winHeight;

	bool m_needsUpdate = false;
	bool m_softShadows = false;
	int m_samples = 16;
	float m_lightRadius = 0.5;
};
