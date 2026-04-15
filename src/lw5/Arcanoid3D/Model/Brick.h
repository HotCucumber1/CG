#pragma once
#include "../../../Common/Component/Vector.h"

class Brick
{
public:
	Brick(
		const Vector3f& position,
		const Vector3f& size,
		int hitPoints,
		const Vector3f& color);

	bool IsDestroyed() const
	{
		return m_hitPoints <= 0;
	}

	void Hit()
	{
		if (m_hitPoints > 0)
		{
			m_hitPoints--;
		}
	}

	int GetHitPoints() const
	{
		return m_hitPoints;
	}

	Vector3f GetPosition() const
	{
		return m_position;
	}

	Vector3f GetSize() const
	{
		return m_size;
	}

	Vector3f GetColor() const
	{
		return m_color;
	}

	bool CheckCollision(const Vector3f& ballPos, float ballRadius, Vector3f& outNormal) const;

private:
	int m_hitPoints;
	Vector3f m_position;
	Vector3f m_size;
	Vector3f m_color;
};