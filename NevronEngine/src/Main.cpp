#include "Utility.h"
#include "Time.h"
#include "Timer.h"
#include "..\Math\Math.h"
#include "Systemdefs.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <thread>

using namespace std::chrono_literals;
using namespace Utility;
using namespace Math;

struct ShaderSource
{
	ShaderSource(std::stringstream ss[2]) : vertexSource(ss[0].str()), fragmentSource(ss[1].str()) {};
	std::string vertexSource, fragmentSource;
	std::string operator[](unsigned int index) { return *(&vertexSource + index); };
};

ShaderSource ParseShader(const std::string& shaderName)
{
	std::string filepath = FindFile(shaderName + ".shader");
	if (filepath == "")
		Log("Could find shader " + shaderName);
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};


	std::ifstream stream(filepath);
	std::string line;

	ShaderType type = ShaderType::NONE;
	std::stringstream ss[2];
	while (getline(stream, line))
	{
		//Set mode to vertex shader
		if (line.find("#VERTEX_SHADER") != std::string::npos)
			type = ShaderType::VERTEX;

		//Set mode to vertex shader
		else if (line.find("#FRAGMENT_SHADER") != std::string::npos)
			type = ShaderType::FRAGMENT;
		else
		{
			ss[(int)type] << line << "\n";
		}
	}
	ShaderSource sources(ss);

	if (sources.vertexSource.size() == 0)
		Error(1);
	if (sources.fragmentSource.size() == 0)
		Error(2);

	return sources;
}

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

		char* message = new char[length * sizeof(char)];
		glGetShaderInfoLog(id, length, &length, message);
		Log((message), type == GL_VERTEX_SHADER ? "vertex" : "fragment" + std::string(" Shader Error"));
		glDeleteShader(id);
		delete[] message;
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	Log("Compiled vertex shader");

	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	Log("Compiled fragment shader");

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	//Delete inscalarediates
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(int argc, char** argv)
{
	system("color a");

	Time::Init();

	setWorkingDir(DirectoryUp(argv[0]));

	Log(APPNAME " initializing...");

	Timer* timer = new Timer("Startup");


	LoadErrorDef();

	AddError("Could not init glew", 0);

	//Initialize the library
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create a windowed mode window and its OpenGL context
	GLFWwindow* window = glfwCreateWindow(640, 480, APPNAME, NULL, NULL);

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
		Log("Could not init glew", "Fatal Error");
		return -1;
	}

	//During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(ErrorCallBack, 0);

	Log("OpenGl version: " + std::string((char*)glGetString(GL_VERSION)), "OpenGL");

	delete timer;


	mat4 matrix({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
	Vector3 euler(DEG_90, 0, 0);
	matrix = mat4::RotateYawPitchRoll(euler);
	Quaternion quaternion(euler);
	Vector3 vector(0, 0, 1);


	Log(mat4::RotateYawPitchRoll({ DEG_90,0,0 }) * vector, "MatrixX");
	Log(Quaternion({ DEG_90, 0, 0 }) * vector, "QuaternionX");


	Log(mat4::RotateYawPitchRoll({ 0,DEG_90,0 }) * vector, "MatrixY");
	Log(Quaternion({ 0,DEG_90, 0, }) * vector, "QuaternionY");

	Log(mat4::RotateYawPitchRoll({ 0,0,DEG_90 }) * vector, "MatrixZ");
	Log(Quaternion({ 0, 0,DEG_90 }) * vector, "QuaternionZ");


	Log(mat4::RotateYawPitchRoll({ 0,DEG_45,DEG_90 }) * vector, "MatrixZ");
	Log(Quaternion({ 0, DEG_45,DEG_90 }) * vector, "QuaternionZ");

	//Log(vector);

	//Simple square
	float positions[8] = {
		-0.5f, -0.5f, //0
		 0.5f,  -0.5f, //1
		 0.5f, 0.5f,//2
		 -0.5f, 0.5f//3
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	//Vertex buffer
	VertexArray va;
	VertexBuffer vb(positions, 4 * 2 * sizeof(float), true);

	//Vertex layout
	VertexBufferLayout layout;
	layout.Push<float>(2);

	va.AddBuffer(vb, layout);

	//Index buffer
	IndexBuffer ib(indices, 6);

	//Shaders
	ShaderSource shaderSources = ParseShader("Basic");
	if (shaderSources[0] == "" || shaderSources[1] == "")
		Log("One or more shaders failed to compile");

	unsigned int shader = CreateShader(shaderSources.vertexSource, shaderSources.fragmentSource);

	GLCall(glUseProgram(shader)); //Setting shader

	//Assigning uniforms
	int u_color = glGetUniformLocation(shader, "u_color");
	ASSERT(u_color != -1);

	Log("----------Entering game loop----------\n", "Main");
	//Loop until the user closes the window
	float list1[5] = { 1,2,3,4,5 };
	while (!glfwWindowShouldClose(window))
	{
		vec vector(5);
		vector = list1;
		
		//mat matrix(100000, 100000);
		/*if (Time::frameCount % 30 == 0)
			Log(STR(Time::frameRate), "Framerate");*/
		Time::Update();
		glClear(GL_COLOR_BUFFER_BIT);
		//Binding
		va.Bind();
		ib.Bind();
		GLCall(glUseProgram(shader));

		vec3 color = vec3::HSV(Time::elapsedTime / 2.0f, 1, 1);
		glUniform4f(u_color, color.x, color.y, color.z, 1.0f);
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		//Swap front and back buffers
		glfwSwapBuffers(window);

		//Poll for and process events
		glfwPollEvents();

		//Unbinding
		va.Unbind();
		GLCall(glUseProgram(0));
		vb.Unbind();
		ib.Unbind();
	}

	Log("Closing Window", "Main");

	SaveErrorDef();

	glDeleteProgram(shader);
	glfwTerminate();

	Log("Program has terminated correctly");
	return 0;
}