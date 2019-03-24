#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Utility.h"
#include "Time.h"
#include "Timer.h"
#include <iostream>
#include <string>

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* shaderSource = source.c_str();
	glShaderSource(id, 1, &shaderSource, nullptr);
	glCompileShader(id);

	//Error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	//There was a compile error
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		Util::Log((message), type == GL_VERTEX_SHADER ? "vertex" : "fragment" + std::string(" Shader Error"));
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	Util::Log("Compiled vertex shader");

	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	Util::Log("Compiled fragment shader");

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	//Delete intermediates
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(int argc, char** argv)
{

	Time::Init();
	Timer* timer = new Timer("Startup");

	system("color a");

	Util::setWorkingDir(argv[0]);
	Util::SaveErrorDef();

	Util::AddError("Could not init glew", 0);

	//Initialize the library
	if (!glfwInit())
		return -1;



	//Create a windowed mode window and its OpenGL context
	GLFWwindow*	window = glfwCreateWindow(640, 480, "Aquelos Engine", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	//Make the window's context current
	glfwMakeContextCurrent(window);

	//Initialize glew and fetch opengl functions
	if (glewInit() != GLEW_OK)
	{
		Util::Error(0);
	}

	std::cout << "OpenGl version: " << glGetString(GL_VERSION) << std::endl;
	delete timer;

	float positions[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f };


	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//Put data in buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);

	//glBindBuffer(GL_ARRAY_BUFFER, buffer);

	std::string vertexShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = position;\n"
		"}\n";

	std::string fragmentShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n";

	unsigned int shader = CreateShader(vertexShader, fragmentShader);

	glUseProgram(shader);
	//Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		Time::Update();
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 3, );


		//Swap front and back buffers
		glfwSwapBuffers(window);

		//Poll for and process events
		glfwPollEvents();
	}
	Utility::Log("Closing Window", "Terminator");

	Util::SaveErrorDef();

	glfwTerminate();

	Util::Log("Program has terminated correctly");
	return 0;
}