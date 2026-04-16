#include "GameModel.h"

const Level::Layout FIRST_LEVEL = { {
	{ 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1 },
} };
const Level::Layout SECOND_LEVEL = { {
	{ 3, 3, 3, 3, 3 },
	{ 2, 2, 2, 2, 2 },
	{ 1, 1, 1, 1, 1 },
} };
const Level::Layout THIRD_LEVEL = { {
	{ 1, 2, 3, 2, 1 },
	{ 2, 3, 4, 3, 2 },
	{ 1, 2, 3, 2, 1 },
} };

GameModel::GameModel()
{
	m_levelLayouts.push_back(FIRST_LEVEL);
	m_levelLayouts.push_back(SECOND_LEVEL);
	m_levelLayouts.push_back(THIRD_LEVEL);

	LoadNextLevel();
	ResetBall();
}

Event GameModel::Update(const float deltaTime, const float paddleMoveDirection)
{
	bool brickDestroyed = false;
	if (m_gameOver || m_levelCompleted)
	{
		return {};
	}

	constexpr float fieldLeft = -FIELD_WIDTH / 2;
	constexpr float fieldRight = FIELD_WIDTH / 2;
	m_paddle.Move(paddleMoveDirection, deltaTime, fieldLeft, fieldRight);

	const Vector2f paddleBoundsX(m_paddle.GetPosition().x, 0);
	const auto ballReflected = m_ball.Update(deltaTime, paddleBoundsX, m_paddle.GetPosition().y, m_paddle.GetHalfWidth());

	if (m_currentLevel)
	{
		brickDestroyed = m_currentLevel->UpdateBricksCollision(m_ball);
	}
	if (m_ball.GetPosition().y - m_ball.GetRadius() < -FIELD_HEIGHT / 2)
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
	}
	return {
		brickDestroyed,
		ballReflected
	};
}

void GameModel::ResetBall()
{
	const Vector3f up(0, 1, 0);
	m_ball.SetPosition(Vector3f(m_paddle.GetPosition().x, m_paddle.GetPosition().y + m_ball.GetRadius(), 0));
	m_ball.SetVelocity(up * m_ball.GetSpeed());
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
	m_currentLevel = std::make_unique<Level>();
	m_currentLevel->LoadFromLayout(m_levelLayouts[m_currentLevelIndex]);
}
