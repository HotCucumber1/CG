#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

void Draw(int width, int height);

int main()
{
	try
	{
		if (!glfwInit())
		{
			std::cerr << "Ошибка инициализации GLFW" << std::endl;
			return EXIT_FAILURE;
		}

		constexpr int width = 500;
		constexpr int height = 500;

		const auto window = glfwCreateWindow(
			width,
			height,
			"OpenGL Window",
			nullptr,
			nullptr);
		if (!window)
		{
			std::cerr << "Ошибка создания окна" << std::endl;
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);

		while (!glfwWindowShouldClose(window))
		{
			Draw(width, height);
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwDestroyWindow(window);
		glfwTerminate();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}

void Draw(const int width, const int height)
{
	glClearColor(1, 0.38, 0.24, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, width, height);

	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0);
	glVertex2f(-0.8, 0.8);

	glColor3f(0, 1, 0);
	glVertex2f(0, -0.8);

	glColor3f(0, 0, 1);
	glVertex2f(0.8, 0.8);
	glEnd();
}
