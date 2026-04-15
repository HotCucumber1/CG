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
	GLint m_lightPosLoc = -1;
	GLint m_lightColorLoc = -1;
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

	GLint m_modelLoc = -1;
	GLint m_viewLoc = -1;
	GLint m_projectionLoc = -1;

	GLint m_viewPosLoc = -1;
	GLint m_objectColorLoc = -1;
	GLint m_materialDiffuseLoc = -1;

	Vector3f m_lightPos = { 5, 10, 5 };
	Vector3f m_lightColor = { 1, 1, 1 };
};
