#include "../../Common/GLFWInitializer/GLFWInitializer.h"
#include "Shape.h"
#include <iostream>

constexpr int WINDOW_WIDTH = 1000;
constexpr int WINDOW_HEIGHT = 700;

int main()
{
	try
	{
		GLFWInitializer initializer;
		Shape shapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Shape");
		shapeWindow.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
}