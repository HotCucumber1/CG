#pragma once
#include "../../Common/BaseWindow/BaseWindow.h"
#include "IView.h"

class AsteroidsView final
	: public BaseWindow
	, public IView
{
public:
	AsteroidsView(
		int width,
		int height,
		const std::string& title);

private:
	void Draw(int width, int height) override;

	void DrawShip() const;

	void DrawWings() const;

	void DrawFire() const;

	void DrawBullets() const;

	void DrawAsteroids() const;

	void DrawHealth() const;

	void DrawStars() const;
};
