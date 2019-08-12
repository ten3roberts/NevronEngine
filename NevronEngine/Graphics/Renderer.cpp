#include "Renderer.h"
#include "..\src\Utility.h"
using namespace Utility;

void GLAPIENTRY ErrorCallBack(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	{

		std::string msg = "type: " + STR(type) + ", severity: " + STR(severity) + ", message: " + message;
		LogS("OpenGL Callback", msg);
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
		LogS("OpenGL Error", ("0x" + STR(error) + "; Function: " + function + " " + getFilename(file, true) + ":" + STR(line)));
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



void Renderer::Clear(Vector4 color) const
{
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const
{
	shader.Bind();

	vertexArray.Bind();
	indexBuffer.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr));

	//Unbinding for debug; otherwise 

}

void Renderer::Draw(Model* model, const Shader& shader) const
{
	shader.Bind();

	VertexArray* va = model->getVertexArray();
	IndexBuffer* ib = model->getIndexBuffer();

	va->Bind();
	ib->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib->getCount(), GL_UNSIGNED_INT, nullptr));

	//Unbinding for debug; otherwise 

}

