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
		return Vector3f(m_halfWidth * 2, m_halfHeight * 2, m_halfDepth * 2.0f);
	}

	float GetHalfWidth() const
	{
		return m_halfWidth;
	}

	void SetPosition(const Vector3f& pos)
	{
		m_position = pos;
	}

private:
	Vector3f m_position;
	float m_halfWidth; // TODO почему half
	float m_halfHeight;
	float m_halfDepth;
	float m_speed;
};