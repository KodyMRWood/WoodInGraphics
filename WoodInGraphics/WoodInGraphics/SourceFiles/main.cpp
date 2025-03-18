/// ---- Summary ----
/// Author: Kody Wood
/// Description: Main cpp. Handles all main fucntions and core loop
/// -----------------
// GLAD must be added before GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#pragma region Shaders

const char *_vertexShaderSource = 
"#version 330 core\n"
"layout (location =0) in vec3 aPos; \n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0f); \n"
"}\0";

const char* _fragmentShaderSource =
"#version 330 core\n"
"out vec4 fragColor; \n"
"void main()\n"
"{\n"
"fragColor = vec4(1.0f,0.5f,0.2f,1.0f); \n"
"}\0";

#pragma endregion

#pragma region Function Declarations
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
#pragma endregion



#pragma region Constants
const unsigned int ScreenWidth = 800;
const unsigned int ScreenHeight = 600;
const char WindowName[15] = "WoodInGraphics";
#pragma endregion



#pragma region Private Variables
float _vertices[] =
{
   -0.5f, -0.5, 0.0f,
	0.5f, -0.5, 0.0f,
	0.0f,  0.5, 0.0f
};
#pragma endregion


/// <summary>
/// Main method
/// </summary>
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create Window
	GLFWwindow* window = glfwCreateWindow(ScreenWidth, ScreenHeight, WindowName, NULL, NULL);
	if (window == NULL)
	{
		std::cout<<"Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Assign resize window callback
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

	// Check Initialization of GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << " Failed to Initialize GLAD" << std::endl;
		return -1;
	}

	// Null Check variables
		// Check if shader compiled successfully
	int success;
	char infolog[512];

	// ---- Vertex Shader ----
	// Create shader object
	// Attach out shader to shader object
	// Compile the shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &_vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
	}

	// ---- Fragment Shader ----
	// Create shader object
	// Attach out shader to shader object
	// Compile the shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &_fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Check if shader compiled successfully
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << std::endl;
	}

	// Bind Shader to Shader object
	// *Warning* Linker errors can occur if typo in shaders, since we link them (glLinkProgram)
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infolog << std::endl;
	}
		// Delete Shaders after linking, no longer needed
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// ---- VBO & VAO ----
	// Definition VertexBufferObjects (VBO) - are pieces of data or objects (buffers) that hold the vertices that will be sent to the shaders.
	// Definition VertexArrayObjects  (VAO) - are pieces of data or objects (buffers) similar to VBOs but VAO hold VBOs and the state in which to send the VBO. So instead of call all VBOs and their states.
	// We can send a VAO and it will send all the VBO and attribs valuies at the same time
	unsigned int VBO, VAO;
	// 1. Generate VAO and VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// 2. Bind VAO
	glBindVertexArray(VAO);
	// 3. Bind Vertices to a VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);
	// 4. Set Attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	// Unbind VBO as it is already bound
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	// Unbind VAO
	glBindVertexArray(0);


	// Run while the window is open (main loop)
	while (!glfwWindowShouldClose(window))
	{
		// Check and call inputs
		processInput(window);

		// Clear Screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Rendering commands
		glUseProgram(shaderProgram);
		// Bind VAO that we want to use
		glBindVertexArray(VAO);
		// Draw triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap buffers
		glfwSwapBuffers(window);
		// Checks and call events
		glfwPollEvents();
	}

	// Cleanup if window closes
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}


#pragma region Private Methods


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
/// Handles input checks
/// </summary>
/// <param name="window"> window that is being interacted with</param>
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
#pragma endregion
