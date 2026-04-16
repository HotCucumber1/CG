#pragma once
#include "Ball.h"
#include "Brick.h"
#include <memory>
#include <vector>

class Level
{
public:
	using Layout = std::vector<
		std::vector<
			std::vector<int>>>;

	Level() = default;

	bool IsCompleted() const;

	void LoadFromLayout(const Layout& layout);

	void UpdateBricksCollision(Ball& ball) const;

	const std::vector<std::unique_ptr<Brick>>& GetBricks() const
	{
		return m_bricks;
	}

private:
	std::vector<std::unique_ptr<Brick>> m_bricks;
	int m_gridWidth;
	int m_gridHeight;
	int m_gridDepth;
};