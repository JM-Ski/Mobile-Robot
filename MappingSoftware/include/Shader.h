#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

class Shader
{
private:
	//Program ID
	GLint programHandle;

public:
	//Constructor
	Shader() {};

	//Main constructor
	Shader(const char* vertexShader, const char* fragmentShader);

	//Load shaders
	void loadShaders(const char* vertexShader, const char* fragmentShader);

	//Get program
	GLint get_Program();
};