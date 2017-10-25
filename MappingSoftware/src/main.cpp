#include <iostream>
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Map.h>
#include <Shader.h>
#include <Texture.h>
#include <vector>
#include <CSVParser.h>
#include <windows.h>

int main()
{
	//Initialize window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 16);

	//Create window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Scattergram Map", nullptr, nullptr);

	//Callbacks = events
	glfwMakeContextCurrent(window);

	//Initialize glew
	glewExperimental = GL_TRUE;
	glewInit();

	//Window size
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	//Load shaders
	Shader shaders;
	shaders.loadShaders("shaders/vertex.vert", "shaders/fragment.frag");
	glEnable(GL_MULTISAMPLE); // Antialiasing

	//Set projection matrix
	glm::mat4 projection = glm::ortho(0.f, 800.f, 600.f, 0.f);

	//Textures - The directory is assets/textures/
	std::vector<Texture> textures;
	textures.push_back(Texture(GL_REPEAT, GL_LINEAR, "map.png", SOIL_LOAD_RGBA, GL_RGBA));
	textures.push_back(Texture(GL_REPEAT, GL_LINEAR, "point.png", SOIL_LOAD_RGBA, GL_RGBA));
	textures.push_back(Texture(GL_REPEAT, GL_LINEAR, "line.png", SOIL_LOAD_RGBA, GL_RGBA));

	CSVParser parser;

	//Create a scattergram map
	Map scattergram = Map(glm::vec2(0.f, 0.f), // Position
		glm::vec2(0.f, 0.f), // Origin
		glm::vec2(800.f, 600.f),  // Size
		0.f, //Angle
		textures.at(0).get_Texture(), //  Map texture
		glm::vec3(1.f, 1.f, 1.f), // Colour
		shaders.get_Program(), //Shader
		textures.at(1).get_Texture(),//Texture for points
		textures.at(2).get_Texture(), //Texture for lines
		glm::vec2(25.f, 25.f), // Boundry 
		8.f); // Unit length for points

	//Windows handles
	WIN32_FIND_DATA h_FileHandle;
	HANDLE h_Find;
	h_Find = FindFirstFile("../Robot/Data/*.csv", &h_FileHandle);

	while (!glfwWindowShouldClose(window))
	{
		//Resize the window
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		//Get any events
		glfwPollEvents();

		//Clear the background
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Try to find next file
		if (h_Find != INVALID_HANDLE_VALUE)
		{
			parser = CSVParser(h_FileHandle.cFileName, false);
			for (int i = 0; i < parser.get_ObstaclePoints().size(); i++) scattergram.add_Point(glm::vec2(parser.get_ObstaclePoints().at(i).x, parser.get_ObstaclePoints().at(i).y), glm::vec3(1.f, 0.f, 0.f));
			for (int i = 0; i < parser.get_RobotPoints().size(); i++) scattergram.add_Point(glm::vec2(parser.get_RobotPoints().at(i).x, parser.get_RobotPoints().at(i).y), glm::vec3(0.f, 0.f, 1.f));
			const char* c_Path = "../Robot/Data/";

			//Throw away this file, becuase we no longer need it
			char ca_Result[100];
			strcpy_s(ca_Result, c_Path);
			strcat_s(ca_Result, h_FileHandle.cFileName);
			remove(ca_Result);
			
			h_Find = INVALID_HANDLE_VALUE; // After removing the file the handle is no longer valid
		}
		else h_Find = FindFirstFile("../Robot/Data/*.csv", &h_FileHandle);
		

		//Use the shader
		glUseProgram(shaders.get_Program());
		//Send this to our vertex shader
		
		GLint projMatLoc = glGetUniformLocation(shaders.get_Program(), "projection");
		glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		//Draw our mapk
		scattergram.draw_Map();

		//Swap screen buffers
		glfwSwapBuffers(window);
	}

	//End
	glfwTerminate();
	return 0;
}