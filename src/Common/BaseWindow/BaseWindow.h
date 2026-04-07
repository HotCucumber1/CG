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

protected:
	static void SetDefaultOrtho(int width, int height, int depth);

private:
	virtual void Draw(int width, int height) = 0;

	virtual void OnRefresh();

	virtual void OnKeyClick(int key, int scancode, int action, int mods);

	virtual void OnCursorPos([[maybe_unused]] double x, [[maybe_unused]] double y);

	virtual void OnMouseButton(int button, int action, int mods);

	static GLFWWindowPtr CreateWindow(int w, int h, const char* title);

	static BaseWindow* GetInstance(GLFWwindow* window);

	static void RefreshCallback(GLFWwindow* window);

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	static void CursorPosCallback(GLFWwindow* window, double x, double y);

protected:
	GLFWWindowPtr m_window;
};
