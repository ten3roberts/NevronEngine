#include "Utility.h"
#include "Settings.h"
#include "Time.h"
#include "Timer.h"
#include "..\Math\Math.h"
#include "Systemdefs.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Logger.h"
#include "Model.h"

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

	Settings* settings = Settings::get();
	settings->Load();

	Log(APPNAME " initializing...");

	Timer* timer = new Timer("Startup");

	//Initialize the library
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create a windowed mode window and its OpenGL context
	GLFWwindow* window = glfwCreateWindow(settings->getScreenWidth(), settings->getScreenHeight(), APPNAME, NULL, NULL);

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
	Vertex vertices[4];
	vertices[0] = Vertex({ -0.5f, -0.5f, 0.0f }, {0.0f, 0.0f});
	vertices[1] = Vertex({ 0.5f, -0.5f, 0.0f }, {1.0f, 0.0f});
	vertices[2] = Vertex({ 0.5f, 0.5f, 0.0f }, {1.0f, 1.0f});
	vertices[3] = Vertex({ -0.5f, 0.5f, 0.0f }, {0.0f, 1.0f});

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);

	//Vertex buffer
	VertexArray va;
	VertexBuffer vb(vertices, sizeof(Vertex)/*sizeof vertex*/ * 4/*Number of vertices*/);

	//Vertex layout
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);

	//Adds vertices
	va.AddBuffer(vb, layout);

	//Index buffer
	IndexBuffer ib(indices, 6);

	//Shaders
	Shader shader("Basic");
	shader.Bind();
	shader.setUniform4f("u_color", Vector4::white);

	Texture iconTex("NevronLogo.png", false);

	GLFWimage icon;
	icon.height = iconTex.getHeight();
	icon.width = iconTex.getWidth();
	icon.pixels = iconTex.getData();


	glfwSetWindowIcon(window, 1, &icon);

	Texture texture("NevronLogo.mc.png");
	Texture texture2("Checker.png");
	texture.Bind();
	shader.setUniform1i("u_texture", 0); //0 is slot

	va.Unbind();

	vb.Unbind();
	ib.Unbind();

	shader.Unbind();

	Renderer renderer;

	//Matrix4 projectionMat = Matrix4::OrthoAspect(2, settings->getAspect(), settings->getScreenNear(), settings->getScreenFar());
	Matrix4 projectionMat = Matrix4::Perspective(settings->getFOV(), settings->getAspect(), settings->getScreenNear(), settings->getScreenFar());

	logger << "Does this work?" << lend;
	logger << author << "Main" << "Hello darkness, " << "can you hear me?";
	logger.end();
	
	Log("----------Entering game loop----------\n", "Main");
	while (!glfwWindowShouldClose(window))
	{
		Time::Update();
		/*if (Time::frameCount % 30 == 0)
			Log(STR(Time::frameRate), "Framerate");*/
		renderer.Clear();

		//Binding

		texture.Bind();
		shader.Bind();

		shader.setUniform1i("u_texture", 0); //0 is slot

		vec3 color = vec3::HSV(Time::elapsedTime / 10.0f, 1, 1);

		shader.setUniform4f("u_color", color);
		//projectionMat = projectionMat.Transpose();

		Quaternion rot({ 0, 1, 0 }, Time::elapsedTime * 1);
		Matrix4 scale = Matrix4::Scale({ 3, 3, 3 });
		Matrix4 translation = Matrix4::Translate({ -0.445, 0, -5 });
		Matrix4 camTranslation = Matrix4::Translate({ 0,Math::Wave(0, 5, 1, Time::elapsedTime) * 0 + 0 ,0 });
		Quaternion camRotation = Quaternion({ 0, 1, 0 }, 0);
		Matrix4 u_MVP = (rot.toMatrix() * scale * translation) * (camRotation.Inverse().toMatrix() * camTranslation) * projectionMat;

		shader.setUniformMat4f("u_MVP", u_MVP);

		renderer.Draw(va, ib, shader);

		Matrix4 u_MVP2 = Quaternion({ 1, 0, 0 }, Time::elapsedTime * 1).toMatrix() * Matrix4::Scale({1, 1, 1}) * Matrix4::Translate({ 0.22, 0, Math::Wave(-25, 0, 0.5, Time::elapsedTime) }) * (camRotation.Inverse().toMatrix() * camTranslation) * projectionMat;
		shader.setUniformMat4f("u_MVP", u_MVP2);
		texture2.Bind();
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

	settings->Save();

	Log("Program has terminated correctly");
	return 0;
}