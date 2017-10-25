#include <Rectangle.h>

Rectangle::Rectangle(glm::vec2 v2_Position_In, glm::vec2 v2_Origin_In, glm::vec2 v2_Size_In, float f_Angle_In, GLuint* ui_Texture_In, glm::vec3 rgb_Colour_In, GLint i_Shader_In)
{
	b_Update = false;

	GLfloat vertices[] = {
		-1.f,  1.f, 0.0f, 0.0f, 1.f,   // Top Left 
		-1.f, -1.f, 0.0f, 0.f, 0.f, // Bottom Left
		1.f,  1.f, 0.0f,  1.f, 1.f,// Top Right
		1.f, -1.f, 0.0f,  1.f, 0.f  // Bottom Right
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3,   // First Triangle
		0, 2, 3    // Second Triangle
	};

	//Generate buffers
	glGenVertexArrays(1, &ui_Vao);
	glGenBuffers(1, &ui_Vbo);
	glGenBuffers(1, &ui_Ebo);

	//Bind vertex buffer
	glBindVertexArray(ui_Vao);
	glBindBuffer(GL_ARRAY_BUFFER, ui_Vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Bind indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ui_Ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Enable it so we can pass it to the shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Stop binding
	glBindVertexArray(0);

	//Init variables
	f_Angle = f_Angle_In;
	v2_Position = v2_Position_In;
	v2_Size = v2_Size_In;
	v2_Origin = v2_Origin_In;
	i_Shader = i_Shader_In;
	rgb_Colour = rgb_Colour_In;
	ui_Texture = ui_Texture_In;

	//Model matrix
	model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(-v2_Origin + (v2_Size / 2.f), 0.f));
	model = glm::translate(model, glm::vec3(v2_Position , 0.f)); // Move to some position
	model = glm::translate(model, -glm::vec3(-v2_Origin + (v2_Size / 2.f), 0.f));
	model = glm::rotate(model, glm::radians(f_Angle), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-v2_Origin + (v2_Size / 2.f), 0.f));
	model = glm::scale(model, glm::vec3(v2_Size / 2.f, 1.f)); // Scale it
}

void Rectangle::update()
{
	if (b_Update) // If it needs updating
	{
		//Model matrix
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(-v2_Origin + (v2_Size / 2.f), 0.f));
		model = glm::translate(model, glm::vec3(v2_Position, 0.f)); // Move to some position
		model = glm::translate(model, -glm::vec3(-v2_Origin + (v2_Size / 2.f), 0.f));
		model = glm::rotate(model, glm::radians(f_Angle), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-v2_Origin + (v2_Size / 2.f), 0.f));
		model = glm::scale(model, glm::vec3(v2_Size / 2.f, 1.f)); // Scale it

		b_Update = false; // We updated it no longer necessary
	}
}

void Rectangle::draw()
{
	//Send model matrix to shader
	GLint modelMatLoc = glGetUniformLocation(i_Shader, "model");
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(model));

	//Colour
	GLint colourLoc = glGetUniformLocation(i_Shader, "colour");
	glUniform3f(colourLoc, rgb_Colour.x, rgb_Colour.y, rgb_Colour.z);

	//Use the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *ui_Texture);

	//Texture location
	GLint textureLoc = glGetUniformLocation(i_Shader, "tex");
	glUniform1i(textureLoc, 0); // Pass it to the shader

	//Bind vertex array and draw the rectangle
	glBindVertexArray(ui_Vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

glm::vec2 Rectangle::get_Position()
{
	return v2_Position;
}

glm::vec2 Rectangle::get_Size()
{
	return v2_Size;
}

void Rectangle::set_Angle(float f_Angle_In)
{
	f_Angle = f_Angle_In;
	b_Update = true;
}

void Rectangle::set_Position(glm::vec2 v2_Position_In)
{
	v2_Position = v2_Position_In;
	b_Update = true;
}