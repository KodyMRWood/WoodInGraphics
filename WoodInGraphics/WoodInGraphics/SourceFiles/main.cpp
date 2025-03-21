/// ---- Summary ----
/// Author: Kody Wood
/// Description: Main cpp. Handles all main fucntions and core loop
/// -----------------
// GLAD must be added before GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Shader.h"
#include "stb_image.h"

#pragma region Function Declarations
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
#pragma endregion Function Declarations



#pragma region Constants
const unsigned int ScreenWidth = 800;
const unsigned int ScreenHeight = 600;
const char WindowName[15] = "WoodInGraphics";
#pragma endregion Constants



#pragma region Private Variables
float _vertices[] =
{	//Vertices		 Color            Texture Coords
	0.5f, 0.5f, 0.0f, 1.0f, 0.0f,0.0f, 1.0f,1.0f, // Top right
	0.5f, -0.5, 0.0f, 0.0f, 1.0f,0.0f, 1.0f,0.0f, // Bottom right
   -0.5f, -0.5, 0.0f, 0.0f, 0.0f,1.0f, 0.0f,0.0f, // Bottom left
   -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,0.0f, 0.0f,1.0f // Top left
};

unsigned int _indices[] =
{
	0,1,3, // Triangle 1
	1,2,3  // Triangle 2
};

float _texCoords[] =
{
	0.0f,0.0f, // Bottom left
	0.0f,1.0f, // Top left
	1.0f,0.0f, // Top right
	1.0f,1.0f  // Bottom right
};



#pragma region Extra Triangles
//float _triangleVertices1[] =
//{
//	-0.75f, 0.75f, 0.0f, // Top left
//	0.0f,  0.75f, 0.0f, // Top right
//   -0.75f, 0.0f, 0.0f, // Bottom left
//};
//
//float _triangleVertices2[] =
//{
//	0.75f, 0.0f, 0.0f, // Top right
//	0.75f, -0.75, 0.0f, // Bottom right
//	0.0f, -0.75, 0.0f, // Bottom left
//};
#pragma endregion

#pragma endregion Private Variables


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
#endif __APPLE__

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


	// Create Shader Object
	Shader shaderObj("SourceFiles/BaseVertexShader.vert", "SourceFiles/BaseFragmentShader.frag");


	// Generate Texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Set Wrapping setings for the S and T axis (texture coords are in STR instead of XYZ)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set Texture filtering for magnifying and minifying
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load Image
	int imgWidth, imgHeight, nrChannel;
	unsigned char* data = stbi_load("Textures/WoodContainer.jpg", &imgWidth, &imgHeight, &nrChannel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Fail to load texture" << std::endl;
	}
	stbi_image_free(data);


	// ---- VBO & VAO ----
	// Definition VertexBufferObjects (VBO) - are pieces of data or objects (buffers) that hold the vertices that will be sent to the shaders.
	// Definition VertexArrayObjects  (VAO) - are pieces of data or objects (buffers) similar to VBOs but VAO hold VBOs and the state in which to send the VBO. So instead of call all VBOs and their states.
	// We can send a VAO and it will send all the VBO and attribs valuies at the same time
	// Definition ElementBufferObject (EBO) - are pieces of data or objects (buffers) that uses indices to determine which vertices to use. Prevents sending overlapping vertices
	unsigned int VBO, VAO, EBO;
	// 1. Generate VAO and VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// 2. Bind VAO
	glBindVertexArray(VAO);
	// 3. Bind Vertices to a VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);
	// 3a. Bind indices to an EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);
	// 4. Set Verteices Attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 4a. Set Color Attributes pointers
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3* sizeof(float)));
	glEnableVertexAttribArray(1);
	// 4a. Set Texture Coordinate Attributes pointers
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	// Unbind VBO as it is already bound
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	// Unbind VAO
	glBindVertexArray(0);

	// Unbind EBO - Always Unbind EBO AFTER unbinding VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


#pragma region Exercise Draw Triangles

	// Triangle 1
	//unsigned int VBOTri, VAOTri;
	//// 1. Generate VAO and VBO
	//glGenVertexArrays(1, &VAOTri);
	//glGenBuffers(1, &VBOTri);
	//// 2. Bind VAO
	//glBindVertexArray(VAOTri);
	//// 3. Bind Vertices to a VBO
	//glBindBuffer(GL_ARRAY_BUFFER, VBOTri);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(_triangleVertices1), _triangleVertices1, GL_STATIC_DRAW);
	//// 4. Set Attributes pointers
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
	//
	//// Triangle 1
	//unsigned int VBOTri2, VAOTri2;
	//// 1. Generate VAO and VBO
	//glGenVertexArrays(1, &VAOTri2);
	//glGenBuffers(1, &VBOTri2);
	//// 2. Bind VAO
	//glBindVertexArray(VAOTri2);
	//// 3. Bind Vertices to a VBO
	//glBindBuffer(GL_ARRAY_BUFFER, VBOTri2);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(_triangleVertices2), _triangleVertices2, GL_STATIC_DRAW);
	//// 4. Set Attributes pointers
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

#pragma endregion Exercise Draw Triangles

	// Run while the window is open (main loop)
	while (!glfwWindowShouldClose(window))
	{
		// Check and call inputs
		processInput(window);

		// Clear Screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Rendering commands
		
		//Set Shader to use
		shaderObj.UseShader();

		// Change Color overtime, and send through uniforms
		//float time = glfwGetTime();
		//float greenValue = (sin(time) * 0.5f) + 0.5f;
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		//// Uniform Exception Check
		//if (vertexColorLocation == -1)
		//{
		//	std::cout << "ERROR::SHADER::UNIFORM_NOT_FOUND\n" << infolog << std::endl;
		//}
		// Send values to uniform at given location
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		// Bind Texture
		glBindTexture(GL_TEXTURE_2D, texture);
		// Bind VAO that we want to use
		glBindVertexArray(VAO);

		// Draw Rectangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

#pragma region Draw triangle Exercise
		////Draw Triangles for exercise
		//// Bind VAO that we want to use
		//glBindVertexArray(VAOTri);
		//// Draw Rectangle
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//// Bind VAO that we want to use
		//glBindVertexArray(VAOTri2);
		//// Draw Rectangle
		//glDrawArrays(GL_TRIANGLES, 0, 3);
#pragma endregion

		// Swap buffers
		glfwSwapBuffers(window);
		// Checks and call events
		glfwPollEvents();
	}

	// Cleanup if window closes
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}
#pragma endregion Private Methods
