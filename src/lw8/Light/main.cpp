#include "../../Common/GLFWInitializer/GLFWInitializer.h"
#include "Window.h"

#include <iostream>

int main()
{
	try
	{
		GLFWInitializer initializer;
		Window app(400, 300, "Ray");
		app.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
}
