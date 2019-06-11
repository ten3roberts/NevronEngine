#include "Renderer.h"
#include "Utility.h"
using namespace Utility;

void GLAPIENTRY ErrorCallBack(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	{

		std::string msg = "type: " + STR(type) + ", severity: " + STR(severity) + ", message: " + message;
		Log(msg, "OpenGL Callback");
		return;
	}
}

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum  error = glGetError())
	{
		Log("0x" + STR(error) + "; Function: " + function + " " + getFilename(file, true) + ":" + STR(line), "OpenGL Error");
		return false;
	}
	return true;
}



Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const
{
	shader.Bind();

	vertexArray.Bind();
	indexBuffer.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr));

	//Unbinding for debug; otherwise 

}
