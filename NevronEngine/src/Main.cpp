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
#include "Transform.h"

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

	logger << APPNAME " initializing..." << lend;

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
		logger << author << "Fatal Error" << "Could not init glew" << lend;
		return -1;
	}

	Texture iconTex("NevronLogo.png", false);

	GLFWimage icon;
	icon.height = iconTex.getHeight();
	icon.width = iconTex.getWidth();
	icon.pixels = iconTex.getData();
	glfwSetWindowIcon(window, 1, &icon);

	//During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(ErrorCallBack, 0);

	logger << "OpenGl version: " + std::string((char*)glGetString(GL_VERSION)) << lend;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	delete timer;

	//Simple square
	Vertex vertices[4];
	vertices[0] = Vertex({ -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f });
	vertices[1] = Vertex({ 0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f });
	vertices[2] = Vertex({ 0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f });
	vertices[3] = Vertex({ -0.5f, 0.5f, 0.0f }, { 0.0f, 1.0f });

	unsigned int indices[6] = {
		0, 1, 2,
		2, 3, 0
	};



	Model model(vertices, 4, indices, 6);

	//Shaders
	Shader shader("Basic");
	shader.Bind();
	shader.setUniform4f("u_color", Vector4::white);

	Texture texture("NevronLogo.mc.png");
	Texture texture2("Checker.png");
	texture.Bind();
	shader.setUniform1i("u_texture", 0); //0 is slot



	shader.Unbind();

	Renderer renderer;

	//Matrix4 projectionMat = Matrix4::OrthoAspect(2, settings->getAspect(), settings->getScreenNear(), settings->getScreenFar());
	Matrix4 projectionMat = Matrix4::Perspective(settings->getFOV(), settings->getAspect(), settings->getScreenNear(), settings->getScreenFar());
	Transform transform1({ 0 }, Quaternion::identity, { 3 });
	Transform transform2;
	logger << author << "Main" << "----------Entering game loop----------\n" << lend;
	while (!glfwWindowShouldClose(window))
	{
		Time::Update();
		if (Time::frameCount % 120 == 0)
			logger << author << "Framerate" << STR(Time::frameRate) << lend;

		renderer.Clear();

		//Binding

		texture.Bind();
		shader.Bind();

		shader.setUniform1i("u_texture", 0); //0 is slot

		vec3 color = vec3::HSV(Time::elapsedTime / 10.0f, 1, 1);

		shader.setUniform4f("u_color", color);
		//projectionMat = projectionMat.Transpose();

		Quaternion rot = Quaternion({ 0,0,1 }, Time::elapsedTime * 2) * Quaternion({ 0, 1, 0 }, Time::elapsedTime * 0.5);
		transform1.position = { -0.445, 0, -5 };
		transform1.rotation = Quaternion({ 0,0,1 }, Time::elapsedTime * 2) * Quaternion({ 0, 1, 0 }, Time::elapsedTime * 0.5);

		Matrix4 camTranslation = Matrix4::Translate({ 0,Math::Wave(0, 5, 1, Time::elapsedTime) * 0 + 0 ,0 });
		Quaternion camRotation = Quaternion({ 0, 1, 0 }, 0);
		transform1.Update();
		Matrix4 u_MVP = /*(rot.toMatrix() * scale * translation)*/ transform1.getWorldMatrix() * (camRotation.Inverse().toMatrix() * camTranslation) * projectionMat;

		shader.setUniformMat4f("u_MVP", u_MVP);

		//renderer.Draw(va, ib, shader);
		renderer.Draw(&model, shader);
		transform2.position = { 0.22, 0, Math::Wave(-25, -0.5, 0.5, Time::elapsedTime) };
		transform2.rotation = Quaternion({ 1, 0, 0 }, Time::elapsedTime * 3);
		transform2.Update();
		Matrix4 u_MVP2 = transform2.getWorldMatrix() * (camRotation.Inverse().toMatrix() * camTranslation) * projectionMat;
		shader.setUniformMat4f("u_MVP", u_MVP2);
		texture2.Bind();

		renderer.Draw(&model, shader);


		//Swap front and back buffers
		glfwSwapBuffers(window);

		//Poll for and process events
		glfwPollEvents();

		//Unbinding
		GLCall(glUseProgram(0));
	}

	logger << ("Closing Window", "Main") << lend;

	glfwTerminate();

	settings->Save();

	logger << "Program has terminated correctly" << lend;
	return 0;
}