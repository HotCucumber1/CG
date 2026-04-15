#include "Level.h"

Level::Level(
	const int width,
	const int height,
	const int depth)
	: m_gridWidth(width)
	, m_gridHeight(height)
	, m_gridDepth(depth)
{
}

bool Level::IsCompleted() const
{
	for (const auto& brick : m_bricks)
	{
		if (!brick->IsDestroyed())
		{
			return false;
		}
	}
	return true;
}

void Level::LoadFromLayout(const Layout& layout)
{
	m_bricks.clear();
	if (layout.empty()
		|| layout[0].empty()
		|| layout[0][0].empty())
	{
		return;
	}

	m_gridDepth = static_cast<int>(layout.size());
	m_gridHeight = static_cast<int>(layout[0].size());
	m_gridWidth = static_cast<int>(layout[0][0].size());

	constexpr auto brickWidth = 1.5f; // TODO в нормальные константы
	constexpr auto brickHeight = 0.8f;
	constexpr auto brickDepth = 0.8f;
	constexpr auto spacing = 0.1f;

	const auto startX = -(m_gridWidth - 1) * (brickWidth + spacing) * 0.5;
	const auto startY = 4.0f;
	const auto startZ = -(m_gridDepth - 1) * (brickDepth + spacing) * 0.5;

	Vector3f brickSize(brickWidth * 0.5, brickHeight * 0.5, brickDepth * 0.5);

	for (int k = 0; k < m_gridDepth; ++k)
	{
		for (int j = 0; j < m_gridHeight; ++j)
		{
			for (int i = 0; i < m_gridWidth; ++i)
			{
				const auto hitPoints = layout[k][j][i];
				if (hitPoints <= 0)
				{
					continue;
				}
				Vector3f position(
					startX + i * (brickWidth + spacing),
					startY - j * (brickHeight + spacing),
					startZ + k * (brickDepth + spacing));

				Vector3f color;
				switch (hitPoints)
				{
				case 1:
					color = Vector3f(0.2f, 0.8f, 0.2f);
					break;
				case 2:
					color = Vector3f(0.9f, 0.9f, 0.2f);
					break;
				case 3:
					color = Vector3f(0.9f, 0.5f, 0.1f);
					break;
				default:
					color = Vector3f(0.9f, 0.1f, 0.1f);
					break;
				}
				m_bricks.push_back(std::make_unique<Brick>(position, brickSize, hitPoints, color));
			}
		}
	}
}

void Level::UpdateBricksCollision(Ball& ball) const
{
	const auto ballPos = ball.GetPosition();
	const auto ballRadius = ball.GetRadius();
	const auto ballVel = ball.GetVelocity();

	for (const auto& brick : m_bricks)
	{
		if (brick->IsDestroyed())
		{
			continue;
		}

		Vector3f normal;
		if (brick->CheckCollision(ballPos, ballRadius, normal))
		{
			brick->Hit();

			float dot = ballVel.Dot(normal);
			if (dot < 0)
			{
				const auto newVelocity = ballVel - normal * (2.0f * dot);
				ball.SetVelocity(newVelocity);
			}

			const auto correctedPos = ballPos + normal * (ballRadius - (ballPos - brick->GetPosition()).GetLength());
			ball.SetPosition(correctedPos);
			break;
		}
	}
}