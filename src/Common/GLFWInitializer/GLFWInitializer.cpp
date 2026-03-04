#include "GLFWInitializer.h"

#include <GLFW/glfw3.h>
#include <stdexcept>

GLFWInitializer::GLFWInitializer()
{
	if (!glfwInit())
	{
		throw std::runtime_error("Failed to init GLFW");
	}
}

GLFWInitializer::~GLFWInitializer()
{
	glfwTerminate();
}