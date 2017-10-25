#include <CSVParser.h>

CSVParser::CSVParser(std::string c_FileName_In, bool b_Swap_In)
{
	std::string s_Path = "../Robot/data/" + c_FileName_In; // Locate the file
	std::ifstream file(s_Path); // Open it

	i_State = 0; // Nothing

	std::string s_Line;
	std::string s_Token = "";

	if (file.is_open()) // Check if file is opened
	{
		while (std::getline(file, s_Line)) // Get new line
		{
			std::istringstream iss(s_Line);

			s_Token = ""; // Clear
			iss >> s_Token;

			if (s_Token == "X,")
			{
				//Do nothing
			}
			else if (s_Token == "o") i_State = State::Obstacle;
			else if (s_Token == "r") i_State = State::Robot;

			//Reset variables
			float i_X = 0;
			float i_Y = 0;

			//Find the right state
			switch (i_State)
			{
			case State::Obstacle: // If obstacle
				if (s_Token[s_Token.length() - 1] == ',') s_Token[s_Token.length() - 1] = '32'; // Get rid of a commna

				i_X = std::atof(s_Token.c_str()); // Convert to int
				iss >> s_Token;
				i_Y = std::atof(s_Token.c_str()); // COnvert to int 

														//Add our points
				if (b_Swap_In) vo_ObstaclePoints.push_back(glm::vec2(i_Y, i_X));
				else vo_ObstaclePoints.push_back(glm::vec2(i_X, i_Y));
				break;

			case State::Robot: // If robot
				if (s_Token[s_Token.length() - 1] == ',') s_Token[s_Token.length() - 1] = '32'; // Get rid of a commna

				i_X = std::atof(s_Token.c_str()); // Convert to int
				iss >> s_Token;
				i_Y = std::atof(s_Token.c_str()); // COnvert to int 

														//Add our points
				if (b_Swap_In) vo_RobotPoints.push_back(glm::vec2(i_Y, i_X));
				else vo_RobotPoints.push_back(glm::vec2(i_X, i_Y));
				break;

			default:
			break;
			}
		}
	}
}

std::vector<glm::vec2> CSVParser::get_ObstaclePoints()
{
	return vo_ObstaclePoints;
}

std::vector<glm::vec2> CSVParser::get_RobotPoints()
{
	return vo_RobotPoints;
}