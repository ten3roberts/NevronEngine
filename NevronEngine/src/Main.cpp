#include "Utility.h"
#include "Settings.h"
#include "Time.h"
#include "Timer.h"
#include <Math.h>
#include "Systemdefs.h"
#include <Graphics/Renderer.h>
#include <Graphics/Texture.h>
#include "Logger.h"
#include "Model.h"
#include "Transform.h"
#include "Rigidbody.h"

#include <Graphics/Shader.h>

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

	Logf("Main", APPNAME " initializing...");

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
		Logf("Fatal Error", "Could not init glew");
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

	Logf("Main", std::string("OpenGl version: " + std::string((char*)glGetString(GL_VERSION))).c_str());

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	delete timer;





	//Model model(vertices, 4, indices, 6);
	Model model = Model::GenerateQuad();

	//Shaders
	Shader shader("Basic");
	Shader multiMapShader("MultiMap");
	shader.Bind();
	shader.setUniform4f("u_color", Vector4::white);

	Material material1("Logo");
	Material material2("Checker");
	Material ground("Ground");

	shader.setUniform1i("u_texture", 0); //0 is slot



	shader.Unbind();

	Renderer renderer;

	//Matrix4 projectionMat = Matrix4::OrthoAspect(2, settings->getAspect(), settings->getScreenNear(), settings->getScreenFar());
	Matrix4 projectionMat = Matrix4::Perspective(settings->getFOV(), settings->getAspect(), settings->getScreenNear(), settings->getScreenFar());
	Transform transform1({ -0.445, 0, -5 }, Quaternion::identity, { 3 });
	Transform transform2;
	Transform transform3;
	Rigidbody rb1;
	Rigidbody rb2;
	Logf("Main", "----------Entering game loop----------\n");

	while (!glfwWindowShouldClose(window))
	{
		Time::Update();
		if (Time::frameCount % 120 == 0)
			Logf("Framerate", STR(Time::frameRate).c_str());

		renderer.Clear();

		//Binding

		material1.color = vec3::HSV(Time::elapsedTime / 10.0f, 1, 1);
		shader.Bind();

		material1.Bind();
		shader.setMaterial(&material1);

		Quaternion rot = Quaternion({ 0,0,1 }, Time::elapsedTime * 2) * Quaternion({ 0, 1, 0 }, Time::elapsedTime * 0.5);
		//transform1.rotation = Quaternion({ 0,0,1 }, Time::elapsedTime * 2) * Quaternion({ 0, 1, 0 }, Time::elapsedTime * 0.5);
		rb1.velocity = Vector3(0.5, 0, -1);
		rb1.angularVelocity = Vector3(1, 2.35, 0);
		rb1.Update(&transform1);

		transform1.Update();

		Matrix4 camTranslation = Matrix4::Translate({ 0,Math::SineWave(-5, 5, 1, Time::elapsedTime) * 0 ,0 });
		Quaternion camRotation = Quaternion({ 0, 1, 0 }, 0);
		Matrix4 u_MVP = /*(rot.toMatrix() * scale * translation)*/ transform1.getWorldMatrix() * (camRotation.Inverse().toMatrix() * camTranslation) * projectionMat;

		shader.setUniformMat4f("u_MVP", u_MVP);

		//renderer.Draw(va, ib, shader);
		renderer.Draw(&model, shader);

		//Object2
		transform2.position = { -2, 0, Math::CosineWave(-25, 25, 1, Time::elapsedTime) };
		transform2.rotation = Quaternion({ 1, 0, 0 }, Time::elapsedTime * 2.8);

		transform2.Update();

		Matrix4 u_MVP2 = transform2.getWorldMatrix() * (camRotation.Inverse().toMatrix() * camTranslation) * projectionMat;
		shader.setUniformMat4f("u_MVP", u_MVP2);
		material2.Bind();
		shader.setMaterial(&material2);
		renderer.Draw(&model, shader);

		//Object3

		multiMapShader.Bind();
		transform3.position = { 0, 0, Math::CosineWave(-1, -5, 1, Time::elapsedTime / 5) };
		transform3.rotation = Quaternion({ 0, 1, 0 }, Time::elapsedTime * 2.8);

		transform3.Update();
		ground.color = vec3::HSV(Time::elapsedTime / 5.0f, 1, 1);
		Matrix4 u_MVP3 = transform3.getWorldMatrix() * (camRotation.Inverse().toMatrix() * camTranslation) * projectionMat;
		multiMapShader.setUniformMat4f("u_MVP", u_MVP3);
		ground.Bind();
		multiMapShader.setMaterial(&ground);
		renderer.Draw(&model, multiMapShader);

		//Swap front and back buffers
		glfwSwapBuffers(window);

		//Poll for and process events
		glfwPollEvents();

		//Unbinding
		shader.Unbind();
	}

	Logf("Main", "Closing Window");

	glfwTerminate();

	settings->Save();

	Logf("Main", "Program has terminated correctly");
	return 0;
}