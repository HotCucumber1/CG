#include "GameView.h"

#include "../../../Common/Shader/Program.h"

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

constexpr auto EYE_POS = glm::vec3(0, -10, 15);
constexpr auto LOOK_AT = glm::vec3(0, 0, 0);
constexpr auto UP = glm::vec3(0, 1, 0);

constexpr auto FOV = glm::radians(50.f);

constexpr auto BALL_RADIUS = 1;
constexpr auto BALL_SEGMENTS = 32;
constexpr auto BALL_RINGS = 16;

std::string LoadShaderSource(const std::string& filepath);

GameView::GameView(const int width, const int height, const char* title)
	: BaseWindow(width, height, title)
{
	m_brickBreakSound.LoadWAV("data/sound/brick.wav");
	m_ballReflectionSound.LoadWAV("data/sound/ball.wav");
	m_mainTheme.LoadWAV("data/sound/main.wav");
	m_mainTheme.Play();
	SetupOpenGL();
}

void GameView::Draw(const int width, const int height)
{
	const auto currentTime = static_cast<float>(glfwGetTime());
	const auto deltaTIme = currentTime - m_lastFrameTime;
	m_lastFrameTime = currentTime;

	const auto events = m_model.Update(deltaTIme, m_paddleMoveDirection);
	if (events.ballReflected)
	{
		m_ballReflectionSound.Play();
	}
	if (events.brickDestroyed)
	{
		m_brickBreakSound.Play();
	}

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	__glewUseProgram(m_program);
	SetProjectionAndView(width, height);
	ApplyLighting();

	RenderBackground();
	RenderBricks();
	RenderPaddle();
	RenderBall();

	if (m_model.IsLevelCompleted())
	{
		m_model.NextLevel();
	}
	glfwSwapBuffers(m_window.get());
}

void GameView::OnKeyClick(const int key, int scancode, const int action, int mods)
{
	if (key == GLFW_KEY_LEFT)
	{
		m_paddleMoveDirection = (action == GLFW_PRESS || action == GLFW_REPEAT) ? -1 : 0;
	}
	else if (key == GLFW_KEY_RIGHT)
	{
		m_paddleMoveDirection = (action == GLFW_PRESS || action == GLFW_REPEAT) ? 1 : 0;
	}
}

void GameView::SetupOpenGL()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	const auto vertexSrc = LoadShaderSource("data/shaders/vertex.glsl");
	const auto fragmentSrc = LoadShaderSource("data/shaders/fragment.glsl");

	m_program.Create(
		vertexSrc.c_str(),
		fragmentSrc.c_str());

	SetupUniformLocations();

	m_ballTexture = Texture("data/textures/ball.png");
	m_paddleTexture = Texture("data/textures/paddle.png");
	m_brickTexture = Texture("data/textures/brick.png");
	m_backgroundTexture = Texture("data/textures/background.png");

	m_cubeMesh = CreateCubeMesh();
	m_sphereMesh = CreateSphereMesh(BALL_RADIUS, BALL_SEGMENTS, BALL_RINGS);
	m_planeMesh = CreatePlaneMesh(BALL_RADIUS, BALL_RADIUS);
}

void GameView::SetupUniformLocations()
{
	m_modelLoc = __glewGetUniformLocation(m_program, "model");
	m_viewLoc = __glewGetUniformLocation(m_program, "view");
	m_projectionLoc = __glewGetUniformLocation(m_program, "projection");
	m_lightPosLoc = __glewGetUniformLocation(m_program, "lightPos");
	m_lightColorLoc = __glewGetUniformLocation(m_program, "lightColor");
	m_viewPosLoc = __glewGetUniformLocation(m_program, "viewPos");
	m_objectColorLoc = __glewGetUniformLocation(m_program, "objectColor");
	m_materialDiffuseLoc = __glewGetUniformLocation(m_program, "material.diffuse");
}

void GameView::RenderBall() const
{
	const auto ball = m_model.GetBall();
	const auto ballPos = ball.GetPosition();

	auto model = glm::translate(
		glm::mat4(1),
		glm::vec3(ballPos.x, ballPos.y, ballPos.z));
	model = glm::scale(model, glm::vec3(ball.GetRadius()));

	__glewUniformMatrix4fv(m_modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	__glewUniform3f(m_objectColorLoc, 1, 1, 1);

	m_ballTexture.Bind();
	__glewUniform1i(m_materialDiffuseLoc, 0);
	m_sphereMesh.Draw();
}

void GameView::RenderPaddle() const
{
	const auto paddle = m_model.GetPaddle();
	const auto pos = paddle.GetPosition();
	const auto size = paddle.GetSize();

	auto model = glm::translate(
		glm::mat4(1),
		glm::vec3(pos.x, pos.y, pos.z));
	model = glm::scale(model, glm::vec3(size.x, size.y, size.z));

	__glewUniformMatrix4fv(m_modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	__glewUniform3f(m_objectColorLoc, 1, 1, 1);

	m_paddleTexture.Bind();
	__glewUniform1i(m_materialDiffuseLoc, 0);
	m_cubeMesh.Draw();
}

void GameView::RenderBricks() const
{
	const auto level = m_model.GetCurrentLevel();
	if (!level)
	{
		return;
	}

	m_brickTexture.Bind();
	__glewUniform1i(m_materialDiffuseLoc, 0);

	for (const auto& brick : level->GetBricks())
	{
		if (brick->IsDestroyed())
		{
			continue;
		}
		const auto pos = brick->GetPosition();
		const auto size = brick->GetSize();

		auto model = glm::translate(glm::mat4(1), glm::vec3(pos.x, pos.y, pos.z));
		model = glm::scale(model, glm::vec3(size.x * 2, size.y * 2, size.z * 2));

		__glewUniformMatrix4fv(m_modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		__glewUniform3f(m_objectColorLoc, 1, 1, 1);

		m_cubeMesh.Draw();
	}
}

void GameView::RenderBackground() const
{
	constexpr float backgroundShift = -5;
	constexpr float bottomY = 0;
	constexpr float centerY = bottomY + (GameModel::FIELD_HEIGHT / 2);

	auto model = glm::translate(glm::mat4(1), glm::vec3(0, centerY, backgroundShift));
	model = glm::scale(model, glm::vec3(GameModel::FIELD_WIDTH, GameModel::FIELD_HEIGHT, 1));
	model = glm::rotate(model, glm::radians(90.f), glm::vec3(1, 0, 0));

	__glewUniformMatrix4fv(m_modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	__glewUniform3f(m_objectColorLoc, 1, 1, 1);

	m_backgroundTexture.Bind(0);
	__glewUniform1i(m_materialDiffuseLoc, 0);
	m_planeMesh.Draw();
}

void GameView::ApplyLighting() const
{
	__glewUniform3f(m_lightPosLoc, m_lightPos.x, m_lightPos.y, m_lightPos.z);
	__glewUniform3f(m_lightColorLoc, m_lightColor.x, m_lightColor.y, m_lightColor.z);
	__glewUniform3f(m_viewPosLoc, EYE_POS.x, EYE_POS.y, EYE_POS.z);
}

void GameView::SetProjectionAndView(const int width, const int height) const
{
	constexpr float nearEdge = 0.1;
	constexpr float farEdge = 100;

	const auto aspectRatio = static_cast<float>(height) / static_cast<float>(width);
	auto projection = glm::perspective(
		FOV,
		aspectRatio,
		nearEdge,
		farEdge);
	auto view = glm::lookAt(EYE_POS, LOOK_AT, UP);

	__glewUniformMatrix4fv(m_projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	__glewUniformMatrix4fv(m_viewLoc, 1, GL_FALSE, glm::value_ptr(view));
}

std::string LoadShaderSource(const std::string& filepath)
{
	std::ifstream file(filepath);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open shader file: " + filepath);
	}
	std::stringstream buffer;
	buffer << file.rdbuf();

	return buffer.str();
}
