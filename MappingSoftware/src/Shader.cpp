#include <Shader.h>

Shader::Shader(const char* vertexShader, const char* fragmentShader)
{
	loadShaders(vertexShader, fragmentShader);
}

void Shader::loadShaders(const char* vertexShader, const char* fragmentShader)
{
	//Streams to store the whole file
	std::stringstream vertexShaderStream;
	std::stringstream fragmentShaderStream;

	//Status of compilation
	int compiled;

	//Sources for openGL
	const char* vertexSource;
	const char* fragmentSource;

	//Files
	std::ifstream openVertex;
	std::ifstream openFragment;

	//Open
	openVertex.open(vertexShader);
	openFragment.open(fragmentShader);

	//If open
	if (openVertex.is_open() && openFragment.is_open())
	{
		//Read the buffer
		vertexShaderStream << openVertex.rdbuf();
		fragmentShaderStream << openFragment.rdbuf();

		//Close them
		openVertex.close();
		openFragment.close();
	}

	std::string vertexShaderStringSource = vertexShaderStream.str();
	std::string fragmentShaderStringSource = fragmentShaderStream.str();

	//Conver from string to C_String
	vertexSource = vertexShaderStringSource.c_str();
	fragmentSource = fragmentShaderStringSource.c_str();

	//Clear them when no longer needed
	vertexShaderStream.clear();
	fragmentShaderStream.clear();

	//Create vertex shader
	GLint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertexSource, nullptr); // Point at the source
	glCompileShader(vertShader); // Compile it

	//Get any compile errors
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint logLength;
		glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			char* log = new char[logLength];
			glGetShaderInfoLog(vertShader, logLength, nullptr, log);
			std::cout << "Vertex shader compilation failure..." << "\n" << log << "\n";

			delete[] log;
		}
	}

	//Create fragment shader
	GLint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragShader);

	//Get any compile errors
	if (!compiled)
	{
		GLint logLength;
		glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			char* log = new char[logLength];
			glGetShaderInfoLog(fragShader, logLength, nullptr, log);
			std::cout << "Fragment shader compilation failure..." << "\n" << log << "\n";

			delete[] log;
		}
	}

	//Program
	programHandle = glCreateProgram();
	glAttachShader(programHandle, vertShader);
	glAttachShader(programHandle, fragShader);
	glLinkProgram(programHandle);

	//Linking errors
	glGetProgramiv(programHandle, GL_LINK_STATUS, &compiled);
	if (!compiled)
	{
		GLint logLength;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			char* log = new char[logLength];
			glGetProgramInfoLog(programHandle, logLength, nullptr, log);
			std::cout << "Program linking failure..." << "\n" << log << "\n";

			delete[] log;
		}
	}

	//We are done now delete everything
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
}

GLint Shader::get_Program()
{
	return programHandle;
}