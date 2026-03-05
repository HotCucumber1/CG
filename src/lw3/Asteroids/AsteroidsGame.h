#pragma once
#include "../../Common/Audio/AudioPlayer.h"
#include "../../Common/BaseWindow/BaseWindow.h"
#include "../../Common/Component/BaseComponent.h"
#include "Entity/Asteroid.h"
#include "Entity/Bullet.h"

#include <queue>

class AsteroidsGame final : public BaseWindow
{
public:
	AsteroidsGame(
		int width,
		int height,
		const std::string& title);

private:
	void Draw(int width, int height) override;

	void DrawShip() const;

	void DrawBullets() const;

	void DrawAsteroids() const;

	void DrawHealth() const;

	void Update();

	void UpdateShip();

	void UpdateBullets();

	void UpdateAsteroids();

	void CalculateAsteroidsBreak();

	void CheckShipCollision();

	void BoundPointPosition(Point& point) const;

	void GenerateAsteroids(size_t count);

	void CreateAsteroid();

	void Respawn();

	void OnKeyClick(int key, int scancode, int action, int mods) override;

private:
	const double RESPAWN_DELAY = 3;

	int m_width;
	int m_height;

	bool m_moveForward = false;
	bool m_rotateLeft = false;
	bool m_rotateRight = false;

	bool m_isAlive = true;
	double m_respawnTimer = 0;
	double m_lastTime = 0;

	double m_rotationAngle = 90;
	int m_health = 3;
	Point m_position;
	Vector m_speed;

	std::deque<Bullet> m_bullets;
	std::vector<Asteroid> m_asteroids;

	AudioPlayer m_shootSound;
	AudioPlayer m_breakSound;
	AudioPlayer m_shipBreakSound;
};
