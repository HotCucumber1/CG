#include "Paddle.h"

#include <algorithm>

Paddle::Paddle(
	const float width,
	const float height,
	const float depth,
	const float speed,
	const Vector3f& startPos)
	: m_position(startPos)
	, m_halfWidth(width * 0.5)
	, m_halfHeight(height * 0.5)
	, m_halfDepth(depth * 0.5)
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

	newX = std::clamp(newX, leftBound + m_halfWidth, rightBound - m_halfWidth);
	m_position.x = newX;
}