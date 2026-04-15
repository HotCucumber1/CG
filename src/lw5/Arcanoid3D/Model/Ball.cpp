#include "Ball.h"

Ball::Ball(
	const float radius,
	const float speed,
	const Vector3f& startPos)
	: m_radius(radius)
	, m_speed(speed)
	, m_position(startPos)
	, m_velocity(Vector3f())
{
}

void Ball::Update(
	const float deltaTime,
	const Vector2f& paddleBoundsX,
	const float paddleY,
	const float paddleHalfWidth)
{
	m_position = m_position + m_velocity * deltaTime; // TODO вынести в GameModel

	constexpr float fieldLeft = -10;
	constexpr float fieldRight = 10;
	constexpr float fieldTop = 8;
	constexpr float fieldFront = -3;
	constexpr float fieldBack = 3;

	if (m_position.x - m_radius < fieldLeft)
	{
		m_position.x = fieldLeft + m_radius;
		ReflectX();
	}
	else if (m_position.x + m_radius > fieldRight)
	{
		m_position.x = fieldRight - m_radius;
		ReflectX();
	}
	if (m_position.y + m_radius > fieldTop)
	{
		m_position.y = fieldTop - m_radius;
		ReflectY();
	}
	if (m_position.z - m_radius < fieldFront)
	{
		m_position.z = fieldFront + m_radius;
		ReflectZ();
	}
	else if (m_position.z + m_radius > fieldBack)
	{
		m_position.z = fieldBack - m_radius;
		ReflectZ();
	}

	if (m_position.y - m_radius < paddleY && m_velocity.y < 0)
	{
		const float paddleTop = paddleY;
		const float paddleLeft = paddleBoundsX.x - paddleHalfWidth;
		const float paddleRight = paddleBoundsX.x + paddleHalfWidth;

		if (m_position.x >= paddleLeft && m_position.x <= paddleRight)
		{
			if (m_position.y - m_radius <= paddleTop)
			{
				m_position.y = paddleTop + m_radius;
				ReflectY();

				const float hitPosition = (m_position.x - paddleBoundsX.x) / paddleHalfWidth;
				m_velocity.x += hitPosition * 3.0f;
				m_velocity = m_velocity.Normalized() * m_speed;
			}
		}
	}

	if (m_velocity.GetLength() > m_speed * 1.5f)
	{
		m_velocity = m_velocity.Normalized() * m_speed;
	}
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
