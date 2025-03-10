/// ---- Summary ----
/// Author: Kody Wood
/// Description: Main cpp. Handles all main fucntions and core loop
/// -----------------


// GLAD must be added before GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

/// <summary>
/// Main method
/// </summary>
void main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	return;
}