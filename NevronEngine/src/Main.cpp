#include "Utility.h"
#include "Time.h"
#include "Timer.h"
#include "..\Math\Math.h"
#include "Systemdefs.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "Shader.h"

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

int main(int argc, char** argv)
{
	system("color a");

	Time::Init();

	setWorkingDir(DirectoryUp(argv[0]));

	Log(APPNAME " initializing...");

	Timer* timer = new Timer("Startup");

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

	//Simple square
	float positions[8] = {
		-0.5f, -0.5f,
		 0.5f,  -0.5f,
		 0.5f, 0.5f,
		 -0.5f, 0.5f
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

	//Adds vertives
	va.AddBuffer(vb, layout);

	//Index buffer
	IndexBuffer ib(indices, 6);

	//Shaders
	Shader shader("Basic");
	shader.Bind();
	shader.SetUniform("u_color", Vector4::red);

	va.Unbind();

	vb.Unbind();
	ib.Unbind();

	shader.Unbind();

	Renderer renderer;

	Log("----------Entering game loop----------\n", "Main");
	while (!glfwWindowShouldClose(window))
	{
		Time::Update();
		/*if (Time::frameCount % 30 == 0)
			Log(STR(Time::frameRate), "Framerate");*/
		renderer.Clear();

		//Binding

		shader.Bind();

		vec3 color = vec3::HSV(Time::elapsedTime / 2.0f, 1, 1);
		shader.SetUniform("u_color", color);

		renderer.Draw(va, ib, shader);

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

	glfwTerminate();

	Log("Program has terminated correctly");
	return 0;
}