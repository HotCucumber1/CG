#pragma once
#include "../../../Common/Component/Vector.h"

class Brick
{
public:
	static constexpr float WIDTH = 1.5;
	static constexpr float HEIGHT = 0.8;
	static constexpr float DEPTH = 0.8;
	static constexpr float SPACING = 0.1;

	Brick(
		const Vector3f& position,
		const Vector3f& size,
		int hitPoints);

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

	bool CheckCollision(const Vector3f& ballPos, float ballRadius, Vector3f& outNormal) const;

private:
	int m_hitPoints;
	Vector3f m_position;
	Vector3f m_size;
};