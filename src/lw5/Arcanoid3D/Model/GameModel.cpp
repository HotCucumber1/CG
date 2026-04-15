#include "GameModel.h"

GameModel::GameModel()
{
	m_levelLayouts.push_back({ {
		{ 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1 },
	} });
	m_levelLayouts.push_back({ {
		{ 3, 3, 3, 3, 3 },
		{ 2, 2, 2, 2, 2 },
		{ 1, 1, 1, 1, 1 },
	} });
	m_levelLayouts.push_back({ {
		{ 1, 2, 3, 2, 1 },
		{ 2, 3, 4, 3, 2 },
		{ 1, 2, 3, 2, 1 },
	} });
	LoadNextLevel();
	ResetBall();
}

void GameModel::Update(const float deltaTime, const float paddleMoveDirection)
{
	if (m_gameOver || m_levelCompleted)
	{
		return;
	}

	constexpr float fieldLeft = -4;
	constexpr float fieldRight = 4;
	m_paddle.Move(paddleMoveDirection, deltaTime, fieldLeft, fieldRight);

	const Vector2f paddleBoundsX(m_paddle.GetPosition().x, 0);
	m_ball.Update(deltaTime, paddleBoundsX, m_paddle.GetPosition().y, m_paddle.GetHalfWidth());

	if (m_currentLevel)
	{
		m_currentLevel->UpdateBricksCollision(m_ball);
	}
	if (m_ball.GetPosition().y - m_ball.GetRadius() < -10)
	{
		m_lives--;
		if (m_lives <= 0)
		{
			m_gameOver = true;
		}
		else
		{
			ResetBall();
		}
	}

	if (m_currentLevel && m_currentLevel->IsCompleted())
	{
		m_levelCompleted = true;
		m_score += 1000 * (m_currentLevelIndex + 1);
	}
}

void GameModel::ResetBall()
{
	m_ball.SetPosition(Vector3f(m_paddle.GetPosition().x, m_paddle.GetPosition().y + m_ball.GetRadius(), 0));
	m_ball.SetVelocity(Vector3f(0, 1, 0).Normalized() * m_ball.GetSpeed());
}

void GameModel::NextLevel()
{
	m_currentLevelIndex++;
	if (m_currentLevelIndex < m_levelLayouts.size())
	{
		LoadNextLevel();
		m_levelCompleted = false;
		ResetBall();
		return;
	}
	m_gameOver = true;
}

void GameModel::LoadNextLevel()
{
	m_currentLevel = std::make_unique<Level>(5, 3, 5);
	m_currentLevel->LoadFromLayout(m_levelLayouts[m_currentLevelIndex]);
}
