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

		const auto absX = std::abs(outNormal.x);
		const auto absY = std::abs(outNormal.y);
		const auto absZ = std::abs(outNormal.z);

		if (absX > absY && absX > absZ)
		{
			outNormal = Vector3f(outNormal.x > 0 ? 1 : -1, 0, 0);
		}
		else if (absY > absX && absY > absZ)
		{
			outNormal = Vector3f(0, outNormal.y > 0 ? 1 : -1, 0.);
		}
		else
		{
			outNormal = Vector3f(0, 0, outNormal.z > 0 ? 1 : -1);
		}
		return true;
	}
	return false;
}
