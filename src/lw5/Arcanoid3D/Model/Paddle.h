#pragma once
#include "../../../Common/Component/Vector.h"

class Paddle
{
public:
	Paddle(float width, float height, float depth, float speed, const Vector3f& startPos);

	void Move(float direction, float deltaTime, float leftBound, float rightBound);

	Vector3f GetPosition() const
	{
		return m_position;
	}

	Vector3f GetSize() const
	{
		return Vector3f(m_width, m_height, m_depth);
	}

	float GetHalfWidth() const
	{
		return m_width / 2;
	}

	void SetPosition(const Vector3f& pos)
	{
		m_position = pos;
	}

private:
	Vector3f m_position;
	float m_width;
	float m_height;
	float m_depth;
	float m_speed;
};