#include "Utility.h"
#include "Settings.h"
#include "Time.h"
#include "Timer.h"
#include <Math.h>
#include "Systemdefs.h"
#include <Graphics/Renderer.h>
#include <Graphics/Texture.h>
#include "Logger.h"
#include <Graphics/Model.h>
#include "Transform.h"
#include "Object.h"
#include <Graphics/Shader.h>
#include <Graphics/UniformBuffer.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <thread>

#include <src/ResourceManager.h>

using namespace std::chrono_literals;
using namespace Utility;
using namespace Math;

int main(int argc, char** argv)
{
	system("color a");


	setWorkingDir(DirectoryUp(argv[0]));

	Settings* settings = Settings::get();
	settings->Load();

	LogS("Main", APPNAME " initializing...");

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
		LogS("Fatal Error", "Could not init glew");
		return -1;
	}

	Texture iconTex("NevronLogo.a", 0, false);
	Texture cursorTex("Arrow", 0, false);

	GLFWimage icon;
	icon.height = iconTex.getHeight();
	icon.width = iconTex.getWidth();
	icon.pixels = iconTex.getData();
	glfwSetWindowIcon(window, 1, &icon);

	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);

	//During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(ErrorCallBack, 0);

	LogS("Main", std::string("OpenGl version: " + std::string((char*)glGetString(GL_VERSION))));

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	delete timer;



	ResourceManager* rscManager = ResourceManager::Get();
	//Model model(vertices, 4, indices, 6);
	Model model = Model::GenerateQuad();

	//Shaders
	rsc<Shader> shader = rscManager->GetShader("Basic");
	//Shader shader("Basic");
	Shader multiMapShader("MultiMap");
	shader->Bind();
	shader->setUniform4f("u_color", Vector4::white);

	Material material1("Logo");
	rsc<Material> ground = rscManager->GetMaterial("Ground");
	shader->setUniform1i("u_texture", 0); //0 is slot
	shader->Bind();
	//Uniform buffer objects

	struct EnvironmentType
	{
		vec3 color;
		float viewDistance;
	};

	EnvironmentType environment;
	environment.color = vec3::red;
	environment.viewDistance = 10;

	UniformBuffer ubo("Environment", nullptr, sizeof(EnvironmentType));
	ubo.setData(&shader, &environment, sizeof(EnvironmentType));

	shader->Unbind();

	Renderer renderer;

	//Matrix4 projectionMat = Matrix4::OrthoAspect(2, settings->getAspect(), settings->getScreenNear(), settings->getScreenFar());
	Matrix4 projectionMat = Matrix4::Perspective(settings->getFOV(), settings->getAspect(), settings->getScreenNear(), settings->getScreenFar());
	Transform transform1({ -0.445, 0, -5 }, Quaternion::identity, { 3 });
	Transform transform2;
	Transform transform3;
	Rigidbody rb1;
	Rigidbody rb2;
	LogS("Main", "----------Entering game loop----------\n");




	rsc<Material> material2 = rscManager->GetResource<Material>("Mario");

	Object object;

	object.AddComponent<Shader>("Basic");
	while (!glfwWindowShouldClose(window))
	{
		Time::Update();
		if (Time::frameCount % 10 == 0)
			glfwSetWindowTitle(window, format("%c fps: %d", APPNAME, (int)Time::frameRate).c_str());
		object.AddComponent<Shader>("Basic2");
		object.RemoveComponent<Shader>();
		//object.AddComponent<Shader>("Basic2");
		//rscManager->GetShader("Basic2");
		//LogS("Basic shader references", "references: %d", object.GetComponent<Shader>().getReferenceCount());

		renderer.Clear();

		//Binding

		material1.color = vec3::HSV(Time::elapsedTime / 10.0f, 1, 1);
		shader->Bind();

		material1.Bind();
		shader->setMaterial(&material1);

		Quaternion rot = Quaternion({ 0,0,1 }, Time::elapsedTime * 2) * Quaternion({ 0, 1, 0 }, Time::elapsedTime * 0.5);
		//transform1.rotation = Quaternion({ 0,0,1 }, Time::elapsedTime * 2) * Quaternion({ 0, 1, 0 }, Time::elapsedTime * 0.5);
		rb1.velocity = Vector3(0.5, 0, -1);
		rb1.angularVelocity = Vector3(1, 2.35, 0);
		rb1.Update(&transform1);
		Vector list = Random::Array(5);
		//LogS("", "Entity 1 pos %vn", list);
		transform1.Update();

		Matrix4 camTranslation = Matrix4::Translate({ 0,Math::SineWave(-5, 5, 1, Time::elapsedTime) * 0 ,0 });
		Quaternion camRotation = Quaternion({ 0, 1, 0 }, 0);
		Matrix4 u_MVP = /*(rot.toMatrix() * scale * translation)*/ transform1.getWorldMatrix() * (camRotation.Inverse().toMatrix() * camTranslation) * projectionMat;

		shader->setUniformMat4f("u_MVP", u_MVP);

		//renderer.Draw(va, ib, shader);
		renderer.Draw(&model, *shader);

		//Object2
		transform2.position = { -2, 0, Math::CosineWave(-25, 25, 1, Time::elapsedTime) };
		transform2.rotation = Quaternion({ 1, 0, 0 }, Time::elapsedTime * 2.8);

		transform2.Update();

		Matrix4 u_MVP2 = transform2.getWorldMatrix() * (camRotation.Inverse().toMatrix() * camTranslation) * projectionMat;
		shader->setUniformMat4f("u_MVP", u_MVP2);
		material2->Bind();
		shader->setMaterial(&material2);
		renderer.Draw(&model, *shader);

		//Object3

		multiMapShader.Bind();
		transform3.position = { 0, 0, Math::CosineWave(-1, -2, 5, Time::elapsedTime) };
		transform3.rotation = Quaternion({ 0, 1, 0 }, Time::elapsedTime);

		transform3.Update();
		//rscManager->GetMaterial("Ground")->color = vec3::HSV(Time::elapsedTime / 5.0f, 1, 1);

		Matrix4 u_MVP3 = transform3.getWorldMatrix() * (camRotation.Inverse().toMatrix() * camTranslation) * projectionMat;
		multiMapShader.setUniformMat4f("u_MVP", u_MVP3);
		ground->Bind();
		multiMapShader.setMaterial(&ground);
		renderer.Draw(&model, multiMapShader);

		//Swap front and back buffers
		glfwSwapBuffers(window);

		//Poll for and process events
		glfwPollEvents();

		//Unbinding
		shader->Unbind();
	}

	LogS("Main", "Closing Window");

	glfwTerminate();

	settings->Save();

	LogS("Main", "Program has terminated correctly");
	return 0;
}


