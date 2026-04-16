#include "Paddle.h"

#include <algorithm>

Paddle::Paddle(
	const float width,
	const float height,
	const float depth,
	const float speed,
	const Vector3f& startPos)
	: m_position(startPos)
	, m_width(width)
	, m_height(height)
	, m_depth(depth)
	, m_speed(speed)
{
}

void Paddle::Move(
	const float direction,
	const float deltaTime,
	const float leftBound,
	const float rightBound)
{
	if (direction == 0)
	{
		return;
	}
	auto newX = m_position.x + direction * m_speed * deltaTime;

	newX = std::clamp(newX, leftBound + m_width / 2, rightBound - m_width / 2);
	m_position.x = newX;
}
