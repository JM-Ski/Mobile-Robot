#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>


class Rectangle
{
protected:
	GLuint ui_Vao, ui_Vbo, ui_Ebo; // Buffers
	GLuint* ui_Texture; // Texture
	glm::mat4 model; // Model matrix
	GLint i_Shader; // Shader
	glm::vec3 rgb_Colour; // Colour of the rectanlge
	glm::vec2 v2_Position; // Position of the rectangle
	glm::vec2 v2_Size; // Size of the rectangle
	glm::vec2 v2_Origin; // Origin of the rectangle
	float f_Angle; // Angle of the rectangle

	bool b_Update; // Should we update?

public:
	//Constructor
	Rectangle() {};

	//Main constructor
	Rectangle(glm::vec2 v2_Position_In, glm::vec2 v2_Origin_In, glm::vec2 v2_Size_In, float f_Angle_In, GLuint* ui_Texture_In, glm::vec3 rgb_Colour_In, GLint i_Shader_In);
	
	//Draw rectangle
	void draw();

	//Updater rectangle
	void update();
	
	//Get position of the rectangle
	glm::vec2 get_Position();

	//Get size of the rectangle
	glm::vec2 get_Size();

	//Get angle of the rectangle
	void set_Angle(float f_Angle_In);

	//Set position of the rectangle
	void set_Position(glm::vec2 v2_Position_In);
};