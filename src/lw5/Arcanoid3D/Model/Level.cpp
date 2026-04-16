#include "Level.h"

#include <iostream>

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

	m_gridDepth = layout.size();
	m_gridHeight = layout[0].size();
	m_gridWidth = layout[0][0].size();

	const auto startX = -(m_gridWidth - 1) * (Brick::WIDTH + Brick::SPACING) / 2;
	constexpr float startY = 6;
	const auto startZ = -(m_gridDepth - 1) * (Brick::DEPTH + Brick::SPACING) / 2;

	Vector3f brickSize(Brick::WIDTH / 2, Brick::HEIGHT / 2, Brick::DEPTH / 2);

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
					startX + i * (Brick::WIDTH + Brick::SPACING),
					startY - j * (Brick::HEIGHT + Brick::SPACING),
					startZ + k * (Brick::DEPTH + Brick::SPACING));

				m_bricks.push_back(std::make_unique<Brick>(
					position,
					brickSize,
					hitPoints));
			}
		}
	}
}

bool Level::UpdateBricksCollision(Ball& ball) const
{
	bool brickDestroyed = false;
	constexpr auto outStep = 0.1;
	const auto ballPos = ball.GetPosition();
	const auto ballRadius = ball.GetRadius();

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
			brickDestroyed = true;

			const auto dot = ball.GetVelocity().Dot(normal);
			if (dot < 0)
			{
				ball.SetVelocity(ball.GetVelocity() - normal * (2 * dot));
			}
			ball.SetPosition(ball.GetPosition() + normal * outStep);
			break;
		}
	}
	return brickDestroyed;
}
