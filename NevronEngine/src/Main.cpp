#include <src/pch.h>
#include "pch.h"
#include "Settings.h"

#include <Graphics/Renderer.h>
#include <Graphics/Texture.h>
#include "Logger.h"
#include <Graphics/Model.h>
#include "Transform.h"
#include "Object.h"
#include <Graphics/Shader.h>
#include <Graphics/UniformBuffer.h>
#include <GLFW/glfw3.h>
#include <src/Camera.h>

#include <iostream>

#include <fstream>
#include <sstream>
#include <thread>

#include <src/ResourceManager.h>

using namespace std::chrono_literals;
using namespace Utility;
using namespace Math;

int main(int argc, char** argv)
{
	//system("color a");
	Time::Init();

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
	glfwWindowHint(GLFW_SAMPLES, Settings::get()->getAASamples());

	glfwSwapInterval(0);

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

	Texture iconTex("NevronLogo.a.png", 0, false);
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
	//glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	if (Settings::get()->getAAEnabled())
		glEnable(GL_MULTISAMPLE);

	delete timer;

	ResourceManager* rscManager = ResourceManager::Get();
	//Model model(vertices, 4, indices, 6);
	/*Model model = *Model::GenerateQuad();

	//Shaders
	rsc<Shader> shader = rscManager->GetShader("Basic");

	rsc<Shader> multiMapShader = rscManager->GetShader("MultiMap");

	shader->Bind();
	shader->setUniform4f("u_color", Vector4::white);

	Material* material1 = &rscManager->GetMaterial("Logo.mc");
	Material* material2 = &rscManager->GetMaterial("Wood");

	rsc<Material> ground = rscManager->GetMaterial("Ground");
	shader->setUniform1i("u_texture", 0); //0 is slot
	shader->Bind();*/
	//Uniform buffer objects

	struct EnvironmentType
	{
		vec3 color;
		float viewDistance;
	};

	EnvironmentType environment;
	environment.color = vec3::red;
	environment.viewDistance = 10;

	//shader->setUniformBuffer("Environment", &environment, sizeof(EnvironmentType));
	//shader->Unbind();

	Renderer* renderer = Renderer::Get();

	Camera camera;
	//Matrix4 projectionMat = Matrix4::OrthoAspect(2, settings->getAspect(), settings->getScreenNear(), settings->getScreenFar());
	//Matrix4 projectionMat = Matrix4::Perspective(settings->getFOV(), settings->getAspect(), settings->getScreenNear(), settings->getScreenFar());

	LogS("Main", "----------Entering game loop----------\n");

	Object object("Basic.glsl", "Quad.dae", "Wood.mat", { new Transform({0,0,0}, Quaternion::identity, 1) });
	Object object2("Basic.glsl", "Quad.dae", "Logo.mat", { new Transform({0,0,-1}, Quaternion::identity, 1), new Rigidbody(0, {0,0,0.5}, 1) });
	Object object3("Basic2.glsl", "Quad.dae", "Logo.mat", { new Transform({0,0,-5}, Quaternion::identity, 1), new Rigidbody(Vector3::right * 0.2f + Vector3::up * 0.1, {0,1,-0.5}, 1) });
	object.AddComponent(new Rigidbody());

	object.rigidbody->velocity = Vector3(0, 0, -1);
	object.rigidbody->angularVelocity = Vector3(0, 1, 0);

	while (!glfwWindowShouldClose(window))
	{
		Time::Update();
		if (Time::frameCount % 10 == 0)
			glfwSetWindowTitle(window, format("%c fps: %d", APPNAME, (int)Time::frameRate).c_str());

		renderer->Clear(Vector4::black);
		//camera.transform.rotation *= Quaternion({ 0, 1, 0.5 }, Time::deltaTime);



		camera.transform.position = { 0,Math::SineWave(-0.5, 0.5, 1, Time::elapsedTime) ,0 };
		camera.Update();
		auto rscMan = ResourceManager::Get();

		//Binding
		object.Update();



		object2.Update();
		object.Render(&camera);

		object3.Update();
		object3.Render(&camera); //drifter

		object2.Render(&camera);
		Renderer::Get()->UnbindShader();
		//Swap front and back buffers
		glfwSwapBuffers(window);

		//Poll for and process events
		glfwPollEvents();
	}


	LogS("Main", "Closing Window");

	glfwTerminate();

	settings->Save();

	LogS("Main", "Program has terminated correctly");
	return 0;
}


