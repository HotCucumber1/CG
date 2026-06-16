#include "App.h"
#include "../../Common/GLFWInitializer/GLFWInitializer.h"
#include <iostream>

int main()
{
	try
	{
		GLFWInitializer initializer;
		GlowApp app(800, 600, "Glow");
		app.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return 0;
}
