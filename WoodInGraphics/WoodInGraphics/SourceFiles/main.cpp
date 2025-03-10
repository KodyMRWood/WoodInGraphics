/// ---- Summary ----
/// Author: Kody Wood
/// Description: Main cpp. Handles all main fucntions and core loop
/// -----------------
// GLAD must be added before GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


// Declare functions
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);



/// <summary>
/// Callback for when the window is resized
/// </summary>
/// <param name="window"> pointer to the window</param>
/// <param name="width"> width of the new window size </param>
/// <param name="height"> height of the new window size </param>
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


/// <summary>
/// Main method
/// </summary>
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create Window
	GLFWwindow* window = glfwCreateWindow(800, 600, "WoodInGraphics", NULL, NULL);
	if (window == NULL)
	{
		std::cout<<"Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Check Initialization of GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << " Failed to Initialize GLAD" << std::endl;
		return -1;
	}

	// Set Viewport dimensions
	glViewport(0, 0, 800, 600);

	// Assign resie window callback
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

	// Run while the window is open (main loop)
	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		// Checks for triggered events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
