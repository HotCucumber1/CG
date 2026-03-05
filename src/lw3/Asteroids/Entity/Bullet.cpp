#include "./Bullet.h"

#include <cmath>

constexpr double BULLET_SPEED = 0.5;

Bullet::Bullet(
	const double x,
	const double y,
	const double angle)
	: m_pos(x, y)
	, m_velocity({
		cos(angle) * BULLET_SPEED,
		sin(angle) * BULLET_SPEED,
	})
{
}

void Bullet::Update()
{
	m_pos.x += m_velocity.x;
	m_pos.y += m_velocity.y;
}

Point Bullet::GetPosition() const
{
	return m_pos;
}
