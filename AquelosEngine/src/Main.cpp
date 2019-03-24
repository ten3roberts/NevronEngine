#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Utility.h"
#include "Time.h"
#include "Timer.h"
#include <iostream>


int main(int argc, char** argv)
{

	Time::Init();
	Timer* timer = new Timer("Startup");

	system("color a");

	Util::setWorkingDir(argv[0]);
	Util::LoadErrorDef();
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
	

	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	//Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		Time::Update();
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_TRIANGLES);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 3, );

		glEnd();

		//Swap front and back buffers
		glfwSwapBuffers(window);

		//Poll for and process events
		glfwPollEvents();
		if (Time::frameCount % 10 == 0)
			Util::Log(std::to_string(Time::frameRate), "FPS");
	}
	Util::Log("Closing Window", "Terminator");

	Util::SaveErrorDef();

	glfwTerminate();

	Util::Log("Program has terminated correctly");
	return 0;
}