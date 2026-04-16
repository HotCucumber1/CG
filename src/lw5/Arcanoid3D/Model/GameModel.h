#pragma once
#include "Ball.h"
#include "Level.h"
#include "Paddle.h"
#include <vector>

struct Event
{
	bool brickDestroyed;
	bool ballReflected;
};

class GameModel
{
public:
	static constexpr float FIELD_WIDTH = 8;
	static constexpr float FIELD_HEIGHT = 10;

	GameModel();

	Event Update(float deltaTime, float paddleMoveDirection);

	void ResetBall();

	Ball GetBall() const
	{
		return m_ball;
	}

	Paddle GetPaddle() const
	{
		return m_paddle;
	}

	const Level* GetCurrentLevel() const // TODO почему указатель?
	{
		return m_currentLevel.get();
	}

	bool IsGameOver() const
	{
		return m_gameOver;
	}

	bool IsLevelCompleted() const
	{
		return m_levelCompleted;
	}

	int GetLives() const
	{
		return m_lives;
	}

	void NextLevel();

private:
	void LoadNextLevel();

private:
	Ball m_ball{ 0.3, 5, Vector3f(0, 1, 0) };
	Paddle m_paddle{ 3, 0.4, 0.8, 5, Vector3f(0.0f, -4.0f, 0.0f) };
	std::unique_ptr<Level> m_currentLevel;
	std::vector<Level::Layout> m_levelLayouts;

	int m_lives = 3;
	bool m_gameOver = false;
	bool m_levelCompleted = false;
	int m_currentLevelIndex = 0;
};
