#pragma once
/// ---- Summary ----
/// Author: Kody Wood
/// Description: Handles shader file reading fdrom disk, compiling, linking and checking errors
/// Everything is done in the header file so it is portable
/// -----------------

#ifndef SHADER_H
#define SHADER_H

#pragma region Includes

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#pragma endregion Includes

class Shader
{
public:

	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void UseShader();

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;

private:

};

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	// Retrieve vertex/fragment source code from file paths
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	//Flagged when: failbit - i.e get number instead of letter | badbit - serious error that disrupts stream
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// Open file
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		// Read file's buffer contexts into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// Close file handlers
		vShaderFile.close();
		fShaderFile.close();
		
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::istream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// Compile Shaders
	// ---- Vertex Shader ----
	// Create shader object
	// Attach out shader to shader object
	// Compile the shader
	unsigned int vertexShader, fragmentShader;
	int success;
	char infoLog[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// ---- Fragment Shader ----
	// Create shader object
	// Attach out shader to shader object
	// Compile the shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	// Check if shader compiled successfully
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Bind Shader to Shader object
	// *Warning* Linker errors can occur if typo in shaders, since we link them (glLinkProgram)
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// Delete Shaders after linking, no longer needed
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

/// <summary>
///  Use the Shader
/// </summary>
void Shader::UseShader()
{
	glUseProgram(ID);
}

/// <summary>
/// Set bool uniform
/// </summary>
/// <param name="name"></param>
/// <param name="value"></param>
void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

#endif // !SHADER_H
