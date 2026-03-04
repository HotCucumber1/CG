#include "../../Common/BaseWindow/BaseWindow.h"
#include "../../Common/GLFWInitializer/GLFWInitializer.h"

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Triangle final : public BaseWindow
{
public:
	Triangle()
		: BaseWindow(500, 500, "My Triangle")
	{
	}

private:
	void Draw(const int width, const int height) override
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
};

int main()
{
	try
	{
		GLFWInitializer initializer;
		Triangle triangle;
		triangle.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
}
