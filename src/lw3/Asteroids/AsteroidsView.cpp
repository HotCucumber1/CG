#include "AsteroidsView.h"

#include "Entity/Bullet.h"

#include <cmath>
#include <thread>

void SetDefaultOrtho(int width, int height);

AsteroidsView::AsteroidsView(
	const int width,
	const int height,
	const std::string& title)
	: BaseWindow(width, height, title.c_str())
{
}

void AsteroidsView::Draw(const int width, const int height)
{
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

void AsteroidsView::DrawShip() const
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

void AsteroidsView::DrawWings() const
{
	glBegin(GL_LINES);
	glColor3f(0.6, 0.6, 0.6);

	glVertex2d(m_position.x - SHIP_WIDTH / 3, m_position.y - SHIP_HEIGHT / 3);
	glVertex2d(m_position.x - SHIP_WIDTH / 1.5, m_position.y - SHIP_HEIGHT / 1.5);

	glVertex2d(m_position.x - SHIP_WIDTH / 3, m_position.y + SHIP_HEIGHT / 3);
	glVertex2d(m_position.x - SHIP_WIDTH / 1.5, m_position.y + SHIP_HEIGHT / 1.5);
	glEnd();
}

void AsteroidsView::DrawFire() const
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

void AsteroidsView::DrawBullets() const
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

void AsteroidsView::DrawAsteroids() const
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

void AsteroidsView::DrawHealth() const
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

void AsteroidsView::DrawStars() const
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

void SetDefaultOrtho(const int width, const int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);

	glMatrixMode(GL_MODELVIEW);
}
