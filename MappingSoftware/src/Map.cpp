#include <Map.h>

Map::Map(glm::vec2 v2_Position_In, glm::vec2 v2_Origin_In, glm::vec2 v2_Size_In, float f_Angle_In, GLuint* ui_MapTexture_In, glm::vec3 rgb_Colour_In, GLint i_Shader_In, GLuint* ui_PointTexture_In, GLuint* ui_LineTexture_In, glm::vec2 v2_Boundry_In, float f_Unit_In) : Rectangle(v2_Position_In, v2_Origin_In, v2_Size_In, f_Angle_In, ui_MapTexture_In, rgb_Colour_In, i_Shader_In)
{
	b_Start = false; // No need to play atm

	v2_StartPos = glm::vec2(v2_Position_In.x + (v2_Size_In.x / 2.f), v2_Position_In.y + (v2_Size_In.y / 2.f)); // Used to place points on the map
	v2_PointSize = v2_Size_In / (v2_Boundry_In * 2.f); // Get the point with the length of 1, Boundry has to be multiplied by two to account for - and + axis
	v2_LineSize = glm::vec2(60.f, 30.f) / f_Unit_In; // Get the reasonable indicator. However, it is stil affected by unit length
	shader = i_Shader_In; // Shader for points
	ui_PointTexture = ui_PointTexture_In; // Texture for points
	v2_Boundry = v2_Boundry_In;

	//Axis lines
	vo_MapLines.push_back(Line(v2_StartPos, glm::vec2(v2_Size_In.x / 2.f, 5.f), glm::vec2(v2_Size_In.x, 10.f), 0.f, ui_LineTexture_In, glm::vec3(0.f, 0.f, 0.f), shader));
	vo_MapLines.push_back(Line(v2_StartPos, glm::vec2(v2_Size_In.y / 2.f, 5.f), glm::vec2(v2_Size_In.y, 10.f), 90.f, ui_LineTexture_In, glm::vec3(0.f, 0.f, 0.f), shader));

	//Indicators for positive X axis
	for (int i = 0; i < v2_Boundry_In.x - 1.f; i++)
	{
		vo_MapLines.push_back(Line(glm::vec2(v2_StartPos.x + ((i + 1) * v2_PointSize.x), v2_StartPos.y),
			v2_LineSize / 2.f,
			v2_LineSize,
			90.f,
			ui_LineTexture_In,
			glm::vec3(0.f, 0.f, 0.f),
			shader));
	}

	//Indicators for negative X axis
	for (int i = 0; i < v2_Boundry_In.x - 1.f; i++)
	{
		vo_MapLines.push_back(Line(glm::vec2(v2_StartPos.x + ((-i - 1) * v2_PointSize.x), v2_StartPos.y),
			v2_LineSize / 2.f,
			v2_LineSize,
			90.f,
			ui_LineTexture_In,
			glm::vec3(0.f, 0.f, 0.f),
			shader));
	}

	//Indicators for positive Y axis
	for (int i = 0; i < v2_Boundry_In.y - 1.f; i++)
	{
		vo_MapLines.push_back(Line(glm::vec2(v2_StartPos.x, v2_StartPos.y - ((i + 1) * v2_PointSize.y)),
			v2_LineSize / 2.f,
			v2_LineSize,
			0.f,
			ui_LineTexture_In,
			glm::vec3(0.f, 0.f, 0.f),
			shader));
	}

	//Indicators for neagtive Y axis
	for (int i = 0; i < v2_Boundry_In.y - 1.f; i++)
	{
		vo_MapLines.push_back(Line(glm::vec2(v2_StartPos.x, v2_StartPos.y - ((-i - 1) * v2_PointSize.y)),
			v2_LineSize / 2.f,
			v2_LineSize,
			0.f,
			ui_LineTexture_In,
			glm::vec3(0.f, 0.f, 0.f),
			shader));
	}
}

void Map::add_Point(glm::vec2 v2_Position_In, glm::vec3 rgb_Colour_In)
{
	//Add the point to the point vector;
	vo_MapPoints.push_back(Point(glm::vec2(v2_StartPos.x + (v2_Position_In.x * v2_PointSize.x), // Calculate the x position on  the map
		v2_StartPos.y - (v2_Position_In.y * v2_PointSize.y)), // Calculate y position on the map
		v2_PointSize / 2.f, // Origin on the point is in the middle
		(v2_PointSize / 20.f), // The size of the point
		0.f, // Angle of  turn
		ui_PointTexture,
		rgb_Colour_In, // Colour
		shader)); // Shader

	b_Start = true; // We added a point so we have to calculate line again
}

void Map::draw_Map()
{
	//Draw a map
	draw();

	//Enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Draw lines first
	for (int i = 0; i < vo_MapLines.size(); i++) vo_MapLines.at(i).draw();

	//Simply draw points on the screen
	for (int i = 0; i < vo_MapPoints.size(); i++) vo_MapPoints.at(i).draw();
	glDisable(GL_BLEND);
}