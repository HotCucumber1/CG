#include "../Common/GLFWInitializer/GLFWInitializer.h"
#include "EdgeDetectionWindow.h"

#include <iostream>
int main()
{
	try
	{
		GLFWInitializer initializer;
		EdgeDetectionWindow app(
			1600, 900, "Filter", "./data/im.jpeg");
		app.Run();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
}
