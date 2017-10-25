#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <SOIL/SOIL.h>
#include <string>

class Texture
{
private:
	GLuint ui_Texture; // Texture ID

public:
	//Constructor
	Texture() {};

	//Main constructor
	Texture(GLint i_WrapMode_In, GLint i_FilterMode_In, std::string s_Name_In, int i_RGBSoilFormat, GLint i_RGBOpenGLFormat);
	
	//Get the texture
	GLuint* get_Texture();
};