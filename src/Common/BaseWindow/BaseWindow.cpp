#include "BaseWindow.h"

#include <stdexcept>

BaseWindow::BaseWindow(
	const int width,
	const int height,
	const char* title)
	: m_window(CreateWindow(width, height, title))
{
	if (!m_window)
	{
		throw std::runtime_error("Failed to create window");
	}
	glfwSetWindowUserPointer(m_window.get(), this);
	glfwSetWindowRefreshCallback(m_window.get(), &BaseWindow::RefreshCallback);
	glfwSetCursorPosCallback(m_window.get(), &BaseWindow::CursorPosCallback);
	glfwSetKeyCallback(m_window.get(), &BaseWindow::KeyCallback);
	glfwSetMouseButtonCallback(m_window.get(), &BaseWindow::MouseButtonCallback);
}

void BaseWindow::Run()
{
	glfwMakeContextCurrent(m_window.get());

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(m_window.get()))
	{
		int width;
		int height;
		glfwGetFramebufferSize(m_window.get(), &width, &height);
		Draw(width, height);
		glfwSwapBuffers(m_window.get());
		glfwPollEvents();
	}
}
void BaseWindow::SetDefaultOrtho(const int width, const int height, const int depth)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -depth, depth);

	glMatrixMode(GL_MODELVIEW);
}

void BaseWindow::OnRefresh()
{
}

void BaseWindow::OnKeyClick(int key, int scancode, int action, int mods)
{
}

void BaseWindow::OnCursorPos(double x, double y)
{
}

void BaseWindow::OnMouseButton(int button, int action, int mods)
{
}

BaseWindow* BaseWindow::GetInstance(GLFWwindow* window)
{
	return static_cast<BaseWindow*>(glfwGetWindowUserPointer(window));
}

void BaseWindow::RefreshCallback(GLFWwindow* window)
{
	if (const auto instance = GetInstance(window))
	{
		instance->OnRefresh();
	}
}

void BaseWindow::KeyCallback(
	GLFWwindow* window,
	const int key,
	const int scancode,
	const int action,
	const int mods)
{
	if (const auto instance = GetInstance(window))
	{
		instance->OnKeyClick(key, scancode, action, mods);
	}
}

void BaseWindow::MouseButtonCallback(
	GLFWwindow* window,
	const int button,
	const int action,
	const int mods)
{
	if (const auto instance = GetInstance(window))
	{
		instance->OnMouseButton(button, action, mods);
	}
}

void BaseWindow::CursorPosCallback(GLFWwindow* window, const double x, const double y)
{
	if (const auto instance = GetInstance(window))
	{
		instance->OnCursorPos(x, y);
	}
}

GLFWWindowPtr BaseWindow::CreateWindow(const int w, const int h, const char* title)
{
	return GLFWWindowPtr(
		glfwCreateWindow(w, h, title, nullptr, nullptr));
}