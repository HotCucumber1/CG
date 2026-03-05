#include "../../Common/GLFWInitializer/GLFWInitializer.h"
#include "AsteroidsGame.h"

#include <iostream>

constexpr int GAME_WIDTH = 1000;
constexpr int GAME_HEIGHT = 700;

int main()
{
	try
	{
		GLFWInitializer initializer;
		AsteroidsGame game(GAME_WIDTH, GAME_HEIGHT, "Asteroids");
		game.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
}