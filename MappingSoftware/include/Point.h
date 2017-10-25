#pragma once
#include <Rectangle.h>

class Point : public Rectangle
{
public:
	//Constructor
	Point() {};

	//Constructor
	Point(glm::vec2 v2_Position_In, glm::vec2 v2_Origin_In, glm::vec2 v2_Size_In, float f_Angle_In, GLuint* ui_Texture_In, glm::vec3 rgb_Colour_In, GLint i_Shader_In);

};