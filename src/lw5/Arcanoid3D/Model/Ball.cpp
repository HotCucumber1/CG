#include "Ball.h"

#include "GameModel.h"

Ball::Ball(
	const float radius,
	const float speed,
	const Vector3f& startPos)
	: m_radius(radius)
	, m_speed(speed)
	, m_position(startPos)
{
}

bool Ball::Update(
	const float deltaTime,
	const Vector2f& paddleBoundsX,
	const float paddleY,
	const float paddleHalfWidth)
{
	bool ballReflected = false;
	m_position = m_position + m_velocity * deltaTime;

	constexpr float fieldLeft = -GameModel::FIELD_WIDTH / 2;
	constexpr float fieldRight = GameModel::FIELD_WIDTH / 2;
	constexpr float fieldTop = GameModel::FIELD_HEIGHT;

	if (m_position.x - m_radius < fieldLeft)
	{
		m_position.x = fieldLeft + m_radius;
		ballReflected = true;
		ReflectX();
	}
	else if (m_position.x + m_radius > fieldRight)
	{
		m_position.x = fieldRight - m_radius;
		ballReflected = true;
		ReflectX();
	}
	if (m_position.y + m_radius > fieldTop)
	{
		m_position.y = fieldTop - m_radius;
		ballReflected = true;
		ReflectY();
	}

	if (m_position.y - m_radius < paddleY && m_velocity.y < 0)
	{
		const float paddleTop = paddleY;
		const float paddleLeft = paddleBoundsX.x - paddleHalfWidth;
		const float paddleRight = paddleBoundsX.x + paddleHalfWidth;

		if (m_position.x >= paddleLeft && m_position.x <= paddleRight && m_position.y - m_radius <= paddleTop)
		{
			m_position.y = paddleTop + m_radius;
			ballReflected = true;
			ReflectY();

			const float hitPosition = (m_position.x - paddleBoundsX.x) / paddleHalfWidth;
			m_velocity.x += hitPosition * 3;
			m_velocity = m_velocity.Normalized() * m_speed;
		}
	}
	return ballReflected;
}

void Ball::ReflectX()
{
	m_velocity.x = -m_velocity.x;
}

void Ball::ReflectY()
{
	m_velocity.y = -m_velocity.y;
}

void Ball::ReflectZ()
{
	m_velocity.z = -m_velocity.z;
}
