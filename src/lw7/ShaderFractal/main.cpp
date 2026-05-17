#include "../../Common/GLFWInitializer/GLFWInitializer.h"
#include "Window.h"

#include <iostream>

int main()
{
	try
	{
		GLFWInitializer initializer;
		Window app(1280, 720, "Mandelbrot Fracta");
		app.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return -1;
	}
	return 0;
}
