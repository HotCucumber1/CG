#pragma once
#include "../../../Common/Component/Vector.h"

class Ball
{
public:
	Ball(float radius, float speed, const Vector3f& startPos);

	bool Update(float deltaTime, const Vector2f& paddleBoundsX, float paddleY, float paddleHalfWidth);

	void ReflectX();

	void ReflectY();

	void ReflectZ();

	Vector3f GetPosition() const
	{
		return m_position;
	}

	float GetRadius() const
	{
		return m_radius;
	}

	float GetSpeed() const
	{
		return m_speed;
	}

	Vector3f GetVelocity() const
	{
		return m_velocity;
	}

	void SetPosition(const Vector3f& pos)
	{
		m_position = pos;
	}

	void SetVelocity(const Vector3f& vel)
	{
		m_velocity = vel;
	}

private:
	float m_radius;
	float m_speed;
	Vector3f m_position;
	Vector3f m_velocity;
};