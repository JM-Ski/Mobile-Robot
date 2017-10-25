#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glm\glm.hpp>
#include <vector>

namespace State
{
	enum State {Obstacle, Robot };
}


class CSVParser
{
private:
	//Points
	std::vector<glm::vec2> vo_ObstaclePoints;
	std::vector<glm::vec2> vo_RobotPoints;

	//State
	int i_State;
public:
	//Constrcutor
	CSVParser() {};
	//Main constructor
	CSVParser(std::string c_FileName_In, bool b_Swap_In = false);
	//Get obstacle points
	std::vector<glm::vec2> get_ObstaclePoints();

	//Get robot points
	std::vector<glm::vec2> get_RobotPoints();
};