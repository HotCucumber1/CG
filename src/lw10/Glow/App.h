#pragma once
#include "../../Common/Shader/Program.h"
#include "../../Common/BaseWindow/BaseWindow.h"
#include "RenderTarget.h"
#include "Quad.h"
#include "Sphere.h"

class GlowApp final : public BaseWindow
{
public:
	GlowApp(int w, int h, const char* title);

private:
	void InitFBOs(int w, int h);

	void DrawScene(const Program& shader, bool renderMask);

	void Draw(int width, int height) override;

	void OnMouseButton(int button, int action, int mods) override;

	void OnCursorPos(double x, double y) override;

	void RenderScene(int width, int height);

	void RenderGlowMask();

	bool RenderGaussianBlur() const;

	void RenderLightMotionBlur(bool horizontal);

	void DrawSun(const Program& shader);

	void DrawEarth(const Program& shader);

	void DrawSphere();

	void InitSphere();

private:
	Program m_sceneShader;
	Program m_blurShader;
	Program m_motionShader;
	Program m_finalShader;

	Sphere m_sphere;
	GLuint m_sphereVAO = 0;

	std::unique_ptr<RenderTarget> m_glowingMaskFBO;
	std::unique_ptr<RenderTarget> m_doubleBlurFBO[2];
	std::unique_ptr<RenderTarget> m_motionBlurFBO[2];
	std::unique_ptr<Quad> m_screenQuad;

	int m_currentMotionIndex = 0;

	float m_cameraAngleX = 0;
	float m_cameraAngleY = 0.5;
	float m_radius = 10;
	double m_lastMouseX = 0;
	double m_lastMouseY = 0;
	bool m_leftMousePressed = false;

	int lastWidth = 0;
	int lastHeight = 0;
};
