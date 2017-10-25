#pragma once
#include <Rectangle.h>

class Line : public Rectangle
{
public:
	//Constructor
	Line() {};

	//Main constructor
	Line(glm::vec2 v2_Position_In, glm::vec2 v2_Origin_In, glm::vec2 v2_Size_In, float f_Angle_In, GLuint* ui_Texture_In, glm::vec3 rgb_Colour_In, GLint i_Shader_In);
};