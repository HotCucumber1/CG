#include "AsteroidsGame.h"

#include <algorithm>
#include <cmath>
#include <random>
#include <thread>

#include "../../Common/Random/Random.h"

constexpr double ROTATION = 0.1;
constexpr int SHIP_HEIGHT = 40;
constexpr int SHIP_WIDTH = 50;
constexpr double FRICTION = 0.004;
constexpr double ACCELERATION = 0.001;
constexpr size_t START_ASTEROIDS_AMOUNT = 4;
constexpr size_t STARS_AMOUNT = 50;

void SetDefaultOrtho(int width, int height);
double DegreeToRad(double angle);
Point GetRandomPoint(int width, int height);

AsteroidsGame::AsteroidsGame(
	const int width,
	const int height,
	const std::string& title)
	: BaseWindow(width, height, title.c_str())
	, m_width(width)
	, m_height(height)
	, m_position(width / 2, height / 2)
	, m_speed(0, 0)
{
	m_shootSound.LoadWAV("./Data/shot.wav");
	m_breakSound.LoadWAV("./Data/break.wav");
	m_shipBreakSound.LoadWAV("./Data/ship_break.wav");

	m_lastTime = glfwGetTime();
	GenerateAsteroids(START_ASTEROIDS_AMOUNT);
	GenerateStars(STARS_AMOUNT);
}

void AsteroidsGame::Draw(const int width, const int height)
{
	Update();

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width, height);

	SetDefaultOrtho(width, height);

	DrawStars();
	glPushMatrix();
	glLoadIdentity();

	glTranslated(m_position.x, m_position.y, 0);
	glRotated(m_rotationAngle, 0, 0, 1);
	glTranslated(-m_position.x, -m_position.y, 0);

	DrawShip();
	glPopMatrix();

	DrawBullets();
	DrawAsteroids();

	DrawHealth();

	if (!m_isAlive && m_health <= 0)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		glfwSetWindowShouldClose(m_window.get(), GLFW_TRUE);
	}
}

void AsteroidsGame::Update()
{
	const double currentTime = glfwGetTime();
	const double deltaTime = currentTime - m_lastTime;
	m_lastTime = currentTime;

	if (!m_isAlive)
	{
		m_respawnTimer -= deltaTime;
		if (m_respawnTimer <= 0)
		{
			Respawn();
		}
	}
	if (m_isAlive)
	{
		UpdateShip();
		CheckShipCollision();
	}
	UpdateBullets();
	UpdateAsteroids();

	CalculateAsteroidsBreak();

	const auto largeAsteroids = std::ranges::count_if(m_asteroids, [](const Asteroid& a) {
		return a.GetSize() == AsteroidSize::Large;
	});
	GenerateAsteroids(START_ASTEROIDS_AMOUNT - largeAsteroids);
}

void AsteroidsGame::DrawShip() const
{
	if (!m_isAlive)
	{
		return;
	}
	DrawWings();

	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0);
	glVertex2d(m_position.x + SHIP_WIDTH / 2, m_position.y);

	glColor3f(0.5, 0.5, 0.5);
	glVertex2d(m_position.x - SHIP_WIDTH / 2, m_position.y - SHIP_HEIGHT / 2);

	glColor3f(0.5, 0.5, 0.5);
	glVertex2d(m_position.x - SHIP_WIDTH / 2, m_position.y + SHIP_HEIGHT / 2);
	glEnd();

	if (m_moveForward)
	{
		DrawFire();
	}
}
void AsteroidsGame::DrawWings() const
{
	glBegin(GL_LINES);
	glColor3f(0.6, 0.6, 0.6);

	glVertex2d(m_position.x - SHIP_WIDTH / 3, m_position.y - SHIP_HEIGHT / 3);
	glVertex2d(m_position.x - SHIP_WIDTH / 1.5, m_position.y - SHIP_HEIGHT / 1.5);

	glVertex2d(m_position.x - SHIP_WIDTH / 3, m_position.y + SHIP_HEIGHT / 3);
	glVertex2d(m_position.x - SHIP_WIDTH / 1.5, m_position.y + SHIP_HEIGHT / 1.5);
	glEnd();
}

void AsteroidsGame::DrawFire() const
{
	constexpr double fireStartFase = 0.8;
	constexpr double fireAmplitude = 30;

	const double time = glfwGetTime();
	const double flicker = fireStartFase + 0.2 * sin(time * 20);
	const double flameLength = fireAmplitude * flicker;

	glBegin(GL_TRIANGLES);

	glColor3f(1, 0.5, 0);
	glVertex2d(m_position.x - SHIP_WIDTH / 2 - flameLength, m_position.y);
	glVertex2d(m_position.x - SHIP_WIDTH / 2, m_position.y - SHIP_HEIGHT / 2);
	glVertex2d(m_position.x - SHIP_WIDTH / 2, m_position.y + SHIP_HEIGHT / 2);

	glColor3f(1, 0.8, 0);
	glVertex2d(m_position.x - SHIP_WIDTH / 2 - flameLength * 0.7, m_position.y);
	glVertex2d(m_position.x - SHIP_WIDTH / 2, m_position.y - SHIP_HEIGHT / 4);
	glVertex2d(m_position.x - SHIP_WIDTH / 2, m_position.y + SHIP_HEIGHT / 4);

	glEnd();
}

void AsteroidsGame::DrawBullets() const
{
	glPointSize(Bullet::SIZE);
	glColor3f(1, 0, 0);

	for (const auto& bullet : m_bullets)
	{
		glBegin(GL_POINTS);
		glVertex2d(bullet.GetPosition().x, bullet.GetPosition().y);
		glEnd();
	}
}

void AsteroidsGame::DrawAsteroids() const
{
	for (const auto& asteroid : m_asteroids)
	{
		glPushMatrix();
		glTranslated(
			asteroid.GetPosition().x,
			asteroid.GetPosition().y,
			0);
		glColor3f(0.5, 0.5, 0.5);
		glBegin(GL_POLYGON);
		for (const auto& point : asteroid.GetLocalPoints())
		{
			glVertex2d(point.x, point.y);
		}
		glEnd();

		glLineWidth(2.);
		glColor3f(0.3, 0.3, 0.3);
		glBegin(GL_LINE_LOOP);
		for (const auto& point : asteroid.GetLocalPoints())
		{
			glVertex2d(point.x, point.y);
		}
		glEnd();
		glPopMatrix();
	}
}

void AsteroidsGame::DrawHealth() const
{
	constexpr int startX = 50;
	constexpr int spaceBetween = 50;
	const int startY = m_height - 50;

	glColor3f(1, 1, 1);
	for (int i = 0; i < m_health; i++)
	{
		glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0);
		glVertex2d(startX + spaceBetween * i + SHIP_HEIGHT / 2, startY);
		glColor3f(0.5, 0.5, 0.5);
		glVertex2d(startX + spaceBetween * i + SHIP_HEIGHT, startY - SHIP_WIDTH);
		glColor3f(0.5, 0.5, 0.5);
		glVertex2d(startX + spaceBetween * i, startY - SHIP_WIDTH);
		glEnd();
	}
}

void AsteroidsGame::DrawStars() const
{
	constexpr double blue = 0.3;
	glPointSize(2);
	glBegin(GL_POINTS);

	for (const auto& star : m_stars)
	{
		glColor3f(star.brightness, star.brightness, blue);
		glVertex2d(star.pos.x, star.pos.y);
	}
	glEnd();
}

void AsteroidsGame::UpdateShip()
{
	if (m_rotateLeft)
	{
		m_rotationAngle += ROTATION;
	}
	if (m_rotateRight)
	{
		m_rotationAngle -= ROTATION;
	}

	if (m_moveForward)
	{
		const auto angle = DegreeToRad(m_rotationAngle);

		m_speed.x += cos(angle) * ACCELERATION;
		m_speed.y += sin(angle) * ACCELERATION;
	}

	m_speed.x *= (1 - FRICTION);
	m_speed.y *= (1 - FRICTION);

	m_position.x += m_speed.x;
	m_position.y += m_speed.y;
	BoundPointPosition(m_position);
}

void AsteroidsGame::UpdateBullets()
{
	for (auto& bullet : m_bullets)
	{
		bullet.Update();
	}

	bool clear = true;
	while (clear)
	{
		if (m_bullets.empty())
		{
			return;
		}
		const auto last = m_bullets.back();
		if (last.GetPosition().x < 0 || last.GetPosition().x > m_width || last.GetPosition().y < 0 || last.GetPosition().y > m_height)
		{
			m_bullets.pop_back();
			continue;
		}
		clear = false;
	}
}

void AsteroidsGame::UpdateAsteroids()
{
	for (auto& asteroid : m_asteroids)
	{
		asteroid.Update();
		asteroid.BoundPosition(m_width, m_height);
	}
}
// TODO звук вынести на событие

// TODO вынести логику столкновения в отдельный класс
void AsteroidsGame::CalculateAsteroidsBreak()
{
	std::vector<Asteroid> newAsteroids;

	auto bulletIt = m_bullets.begin();
	while (bulletIt != m_bullets.end())
	{
		const auto bulletPos = bulletIt->GetPosition();
		bool hit = false;

		auto asteroidIt = m_asteroids.begin();
		while (asteroidIt != m_asteroids.end())
		{
			if (asteroidIt->IsPointInside(bulletPos))
			{
				const auto splitAsteroids = asteroidIt->Split();
				m_breakSound.Play();
				newAsteroids.insert(newAsteroids.end(), splitAsteroids.begin(), splitAsteroids.end());

				m_asteroids.erase(asteroidIt);
				hit = true;
				break;
			}
			++asteroidIt;
		}
		if (hit)
		{
			bulletIt = m_bullets.erase(bulletIt);
		}
		else
		{
			++bulletIt;
		}
	}
	m_asteroids.insert(m_asteroids.end(), newAsteroids.begin(), newAsteroids.end());
}

void AsteroidsGame::CheckShipCollision()
{
	std::vector<Asteroid> newAsteroids;
	std::erase_if(m_asteroids, [this, &newAsteroids](const Asteroid& asteroid) {
		if (asteroid.IsPointInside(m_position))
		{
			const auto splitAsteroids = asteroid.Split();
			newAsteroids.insert(newAsteroids.end(), splitAsteroids.begin(), splitAsteroids.end());
			m_health--;
			m_isAlive = false;
			m_shipBreakSound.Play();
			m_respawnTimer = RESPAWN_DELAY;
			return true;
		}
		return false;
	});

	m_asteroids.insert(
		m_asteroids.end(),
		newAsteroids.begin(),
		newAsteroids.end());
}

void AsteroidsGame::BoundPointPosition(Point& point) const
{
	if (point.x < 0)
	{
		point.x = m_width;
	}
	if (point.x > m_width)
	{
		point.x = 0;
	}
	if (point.y < 0)
	{
		point.y = m_height;
	}
	if (point.y > m_height)
	{
		point.y = 0;
	}
}

void AsteroidsGame::GenerateAsteroids(const size_t count)
{
	for (int _ = 0; _ < count; _++)
	{
		CreateAsteroid();
	}
}

void AsteroidsGame::CreateAsteroid()
{
	const auto center = GetRandomPoint(m_width, m_height);
	const auto angle = DegreeToRad(GetRandomAngle());
	const Vector velocity = {
		cos(angle) * Asteroid::SPEED,
		sin(angle) * Asteroid::SPEED,
	};
	m_asteroids.emplace_back(
		center,
		velocity,
		AsteroidSize::Large,
		Asteroid::BASE_SIZE);
}

void AsteroidsGame::Respawn()
{
	m_position.x = m_width / 2;
	m_position.y = m_height / 2;
	m_speed.x = 0;
	m_speed.y = 0;
	m_rotationAngle = 90;
	m_isAlive = true;
}

void AsteroidsGame::GenerateStars(const int count)
{
	m_stars.clear();
	m_stars.reserve(count);

	for (int _ = 0; _ < count; _++)
	{
		Star star{};
		star.pos = GetRandomPoint(m_width, m_height);
		star.brightness = GetRandomDouble(0.2, 1);
		m_stars.push_back(star);
	}
}

void AsteroidsGame::OnKeyClick(
	const int key,
	int scancode,
	const int action,
	int mods)
{
	const bool isPressed = (action == GLFW_PRESS || action == GLFW_REPEAT);

	switch (key)
	{
	case GLFW_KEY_LEFT:
		m_rotateLeft = isPressed;
		break;
	case GLFW_KEY_RIGHT:
		m_rotateRight = isPressed;
		break;
	case GLFW_KEY_UP:
		m_moveForward = isPressed;
		break;
	case GLFW_KEY_SPACE:
		if (action != GLFW_PRESS)
		{
			break;
		}
		m_shootSound.Play();
		m_bullets.emplace_front(
			m_position.x,
			m_position.y,
			DegreeToRad(m_rotationAngle));
		break;
	default:
		break;
	}
}

void SetDefaultOrtho(const int width, const int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);

	glMatrixMode(GL_MODELVIEW);
}

double DegreeToRad(const double angle)
{
	return angle * M_PI / 180.0;
}

Point GetRandomPoint(const int width, const int height)
{
	static std::uniform_int_distribution distX(0, width);
	static std::uniform_int_distribution distY(0, height);
	return Point(
		distX(GetRandomGenerator()),
		distY(GetRandomGenerator()));
}
