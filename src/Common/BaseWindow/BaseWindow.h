#pragma once
#include <GLFW/glfw3.h>
#include <memory>

using GLFWWindowPtr = std::unique_ptr<GLFWwindow, decltype([](GLFWwindow* window) {
	if (window)
	{
		glfwDestroyWindow(window);
	}
})>;

class BaseWindow
{
public:
	BaseWindow(int width, int height, const char* title);

	BaseWindow(const BaseWindow&) = delete;
	BaseWindow& operator=(const BaseWindow&) = delete;

	void Run();

	virtual ~BaseWindow() = default;

private:
	virtual void Draw(int width, int height) = 0;

	virtual void OnRefresh();

	virtual void OnCursorPos([[maybe_unused]] double x, [[maybe_unused]] double y);

	static GLFWWindowPtr CreateWindow(int w, int h, const char* title);

	static BaseWindow* GetInstance(GLFWwindow* window);

	static void RefreshCallback(GLFWwindow* window);

	static void CursorPosCallback(GLFWwindow* window, double x, double y);

private:
	GLFWWindowPtr m_window;
};
