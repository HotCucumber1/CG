#pragma once

class IView
{
public:
	virtual ~IView() = default;

	virtual void Draw(int width, int height) = 0;
};