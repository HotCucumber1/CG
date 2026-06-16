#include "App.h"
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

GlowApp::GlowApp(const int w, const int h, const char* title)
	: BaseWindow(w, h, title)
	, m_sphere(GetSphere())
{
	const auto sceneVertSource = LoadShaderSource("./data/shaders/scene_vert.glsl");
	const auto sceneFragSource = LoadShaderSource("./data/shaders/scene_frag.glsl");
	const auto quadVertSource = LoadShaderSource("./data/shaders/quad_vert.glsl");
	const auto blurFragSource = LoadShaderSource("./data/shaders/blur_frag.glsl");
	const auto motionFragSource = LoadShaderSource("./data/shaders/motion_blur_frag.glsl");
	const auto mainFragSource = LoadShaderSource("./data/shaders/main_frag.glsl");

	m_sceneShader.Create(sceneVertSource.c_str(), sceneFragSource.c_str());
	m_blurShader.Create(quadVertSource.c_str(), blurFragSource.c_str());
	m_motionShader.Create(quadVertSource.c_str(), motionFragSource.c_str());
	m_finalShader.Create(quadVertSource.c_str(), mainFragSource.c_str());

	m_screenQuad = std::make_unique<Quad>();
}

void GlowApp::InitFBOs(const int w, const int h)
{
	lastWidth = w;
	lastHeight = h;
	int auxW = w / 2;
	int auxH = h / 2;

	m_auxFBO = std::make_unique<RenderTarget>(auxW, auxH, true);
	for (int i = 0; i < 2; i++)
	{
		m_pingpongFBO[i] = std::make_unique<RenderTarget>(auxW, auxH, false);
		m_motionFBO[i] = std::make_unique<RenderTarget>(auxW, auxH, false);

		m_motionFBO[i]->Bind();
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	__glewBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GlowApp::DrawScene(const Program& shader, const bool renderMask)
{
	auto projection = glm::perspective(
		glm::radians(45.0f),
		static_cast<float>(lastWidth) / lastHeight,
		0.1f, 100.0f);

	const float camX = std::sin(m_cameraAngleX) * std::cos(m_cameraAngleY) * m_radius;
	const float camY = std::sin(m_cameraAngleY) * m_radius;
	const float camZ = std::cos(m_cameraAngleX) * std::cos(m_cameraAngleY) * m_radius;
	auto view = glm::lookAt(
		glm::vec3(camX, camY, camZ),
		glm::vec3(0),
		glm::vec3(0, 1, 0));

	__glewUseProgram(shader);
	__glewUniformMatrix4fv(__glewGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	__glewUniformMatrix4fv(__glewGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
	__glewUniform1i(__glewGetUniformLocation(shader, "renderMask"), renderMask);

	DrawSun(shader);
	DrawEarth(shader);
}

void GlowApp::Draw(const int width, const int height)
{
	if (width != lastWidth || height != lastHeight)
	{
		InitFBOs(width, height);
	}

	// --- Шаг 1: Рендер обычной сцены в основной буфер ---
	__glewBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClearColor(0.05f, 0.05f, 0.1f, 1.0f); // Темный космос
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawScene(m_sceneShader, false);

	// --- Шаг 2: Рендер Glow Маски (светящиеся - цвет, остальные - черные) ---
	m_auxFBO->Bind();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawScene(m_sceneShader, true);

	// --- Шаг 3: Размытие (Gaussian Blur) ---
	glDisable(GL_DEPTH_TEST);
	bool horizontal = true, first_iteration = true;
	int amount = 10; // Количество проходов размытия (сильное размытие)
	__glewUseProgram(m_blurShader);
	for (unsigned int i = 0; i < amount; i++)
	{
		m_pingpongFBO[horizontal]->Bind();
		__glewUniform1i(__glewGetUniformLocation(m_blurShader, "horizontal"), horizontal);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? m_auxFBO->GetTexture() : m_pingpongFBO[!horizontal]->GetTexture());
		m_screenQuad->Draw();
		horizontal = !horizontal;
		first_iteration = false;
	}

	// --- Шаг 4: Light Motion Blur (Бонус 50 баллов) ---
	int nextMotionIndex = (m_currentMotionIndex + 1) % 2;
	m_motionFBO[nextMotionIndex]->Bind();
	__glewUseProgram(m_motionShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pingpongFBO[!horizontal]->GetTexture()); // Текущее размытие
	__glewUniform1i(__glewGetUniformLocation(m_motionShader, "currentGlow"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_motionFBO[m_currentMotionIndex]->GetTexture()); // Размытие предыдущего кадра
	__glewUniform1i(__glewGetUniformLocation(m_motionShader, "previousGlow"), 1);

	__glewUniform1f(__glewGetUniformLocation(m_motionShader, "decay"), 0.85f); // Коэффициент затухания инерции

	m_screenQuad->Draw();
	m_currentMotionIndex = nextMotionIndex; // Обновляем индекс для следующего кадра

	// --- Шаг 5: Композитинг на главный экран ---
	__glewBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE); // Аддитивное смешивание

	__glewUseProgram(m_finalShader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_motionFBO[m_currentMotionIndex]->GetTexture()); // Накладываем накопленный моушн-блюр
	__glewUniform1i(__glewGetUniformLocation(m_finalShader, "glowTexture"), 0);

	m_screenQuad->Draw();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void GlowApp::OnMouseButton(const int button, const int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		m_leftMousePressed = (action == GLFW_PRESS);
	}
}

void GlowApp::OnCursorPos(const double x, const double y)
{
	if (m_leftMousePressed)
	{
		m_cameraAngleX -= (x - m_lastMouseX) * 0.01;
		m_cameraAngleY -= (y - m_lastMouseY) * 0.01;
		m_cameraAngleY = glm::clamp(m_cameraAngleY, -1.5f, 1.5f);
	}
	m_lastMouseX = x;
	m_lastMouseY = y;
}

void GlowApp::DrawSun(const Program& shader)
{
	auto model = glm::mat4(1);
	model = glm::scale(model, glm::vec3(1));
	__glewUniformMatrix4fv(__glewGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	__glewUniform3f(__glewGetUniformLocation(shader, "objectColor"), 1.0f, 0.5f, 0.0f);
	__glewUniform1i(__glewGetUniformLocation(shader, "isGlowing"), 1);
	DrawSphere();
}

void GlowApp::DrawEarth(const Program& shader)
{
	auto model = glm::mat4(1.0f);
	const float time = glfwGetTime();
	model = glm::rotate(model, time, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5));

	__glewUniformMatrix4fv(__glewGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	__glewUniform3f(__glewGetUniformLocation(shader, "objectColor"), 0.2, 0.4, 0.8);
	__glewUniform1i(__glewGetUniformLocation(shader, "isGlowing"), 0);
	DrawSphere();
}

void GlowApp::DrawSphere()
{
	if (m_sphereVAO == 0)
	{
		InitSphere();
	}

	glBindVertexArray(m_sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, m_sphere.indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}
void GlowApp::InitSphere()
{
	glGenVertexArrays(1, &m_sphereVAO);

	GLuint vbo;
	GLuint ebo;
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	std::vector<float> data;
	for (unsigned int i = 0; i < m_sphere.positions.size(); ++i)
	{
		data.push_back(m_sphere.positions[i].x);
		data.push_back(m_sphere.positions[i].y);
		data.push_back(m_sphere.positions[i].z);
		if (!m_sphere.normals.empty())
		{
			data.push_back(m_sphere.normals[i].x);
			data.push_back(m_sphere.normals[i].y);
			data.push_back(m_sphere.normals[i].z);
		}
	}

	glBindVertexArray(m_sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_sphere.indices.size() * sizeof(unsigned int), &m_sphere.indices[0], GL_STATIC_DRAW);

	constexpr float stride = 6 * sizeof(float);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
}
