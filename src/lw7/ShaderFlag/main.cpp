#include "Window.h"
#include "../../Common/GLFWInitializer/GLFWInitializer.h"

#include <iostream>

int main()
{
	try {
		GLFWInitializer initializer;
		Window app(1000, 600, "USSR Flag");

		app.Run();
	}
	catch (const std::exception& e) {
		std::cerr << "Application Error: " << e.what() << std::endl;
		return -1;
	}
	return 0;
}
