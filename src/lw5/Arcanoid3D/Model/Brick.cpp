#include "Brick.h"

#include <algorithm>

constexpr float BOTTOM_BOUND = 0.0001;

Brick::Brick(
	const Vector3f& position,
	const Vector3f& size,
	const int hitPoints)
	: m_hitPoints(hitPoints)
	, m_position(position)
	, m_size(size)
{
}

bool Brick::CheckCollision(const Vector3f& ballPos, const float ballRadius, Vector3f& outNormal) const
{
	Vector3f closest;
	closest.x = std::clamp(ballPos.x, m_position.x - m_size.x, m_position.x + m_size.x);
	closest.y = std::clamp(ballPos.y, m_position.y - m_size.y, m_position.y + m_size.y);
	closest.z = std::clamp(ballPos.z, m_position.z - m_size.z, m_position.z + m_size.z);

	const auto delta = ballPos - closest;
	const auto distanceSquared = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;

	if (distanceSquared < ballRadius * ballRadius && distanceSquared > BOTTOM_BOUND)
	{
		const auto distance = std::sqrt(distanceSquared);
		outNormal = delta * (1 / distance);
		return true;
	}
	return false;
}
