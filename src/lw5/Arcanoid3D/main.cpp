#include "View/GameView.h"
#include "../../Common/GLFWInitializer/GLFWInitializer.h"

#include <iostream>

int main()
{
	try
	{
		GLFWInitializer initializer;
		GameView game(1000, 1000, "Arcanoid 3d");

		game.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
}