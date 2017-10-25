#pragma once
#include <Line.h>
#include <Point.h>
#include <vector>
#include <iostream>

class Map : public Rectangle
{
private:
	glm::vec2 v2_Boundry; // Boundry of the X and Y axis
	glm::vec2 v2_StartPos; // The starting position of the map. It will always be the bottom left corner
	std::vector<Point> vo_MapPoints; // Points on the map
	std::vector<Line> vo_MapLines; // Lines on the map
	glm::vec2 v2_PointSize; // Size of the point;
	glm::vec2 v2_PointSize2; // Size of the point;
	glm::vec2 v2_LineSize; // Size of the line
	GLuint* ui_PointTexture; // Texture for points
	GLint shader; // Shader
	float f_Angle; // Angle of the map

	bool b_Start; // Should we start plotting?

public:
	Map() {}; // Default constructor
	Map(glm::vec2 v2_Position_In, glm::vec2 v2_Origin_In, glm::vec2 v2_Size_In, float f_Angle_In, GLuint* ui_Texture_In, glm::vec3 rgb_Colour_In, GLint i_Shader_In, GLuint* ui_PointTexture_In, GLuint* ui_LineTexture_In, glm::vec2 v2_Boundry_In,  float f_Unit_In); // Main constructor
	void add_Point(glm::vec2 v2_Position_In, glm::vec3 rgb_Colour_In); // Add point on the map
	void draw_Map(); // Draw the map
};