#pragma once
#include "../../../Common/Shader/Mesh.h"
#include "../../../Common/BaseWindow/BaseWindow.h"
#include "../../../Common/Shader/Program.h"
#include "../../../Common/Shader/Texture.h"
#include "../Model/GameModel.h"

class GameView final : public BaseWindow
{
public:
	GameView(int width, int height, const char* title);

protected:
	void Draw(int width, int height) override;

	void OnKeyClick(int key, int scancode, int action, int mods) override;

private:
	void SetupOpenGL();

	void SetupUniformLocations(); // TODO не факт

	void RenderBall() const;

	void RenderPaddle();

	void RenderBricks() const;

	void RenderBackground() const;

	void ApplyLighting() const;

	void SetProjectionAndView(int width, int height) const;

private:
	GLint m_lightPosLoc;
	GLint m_lightColorLoc;
	float m_paddleMoveDirection = 0;
	float m_lastFrameTime = 0;

	GameModel m_model;

	Program m_program;
	Texture m_ballTexture;
	Texture m_paddleTexture;
	Texture m_brickTexture;
	Texture m_backgroundTexture;

	Mesh m_cubeMesh;
	Mesh m_sphereMesh;
	Mesh m_planeMesh;

	GLint m_modelLoc;
	GLint m_viewLoc;
	GLint m_projectionLoc;

	GLint m_viewPosLoc;
	GLint m_objectColorLoc;
	GLint m_materialDiffuseLoc;

	Vector3f m_lightPos = { 5, 10, 5 };
	Vector3f m_lightColor = { 1, 1, 1 };
};
