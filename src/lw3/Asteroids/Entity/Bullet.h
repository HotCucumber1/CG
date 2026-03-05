#pragma once
#include "../../../Common/Component/BaseComponent.h"

class Bullet
{
public:
	static constexpr int SIZE = 5;

	Bullet(
		double x,
		double y,
		double angle);

	void Update();

	Point GetPosition() const;

private:
	Point m_pos;
	Point m_velocity;
};