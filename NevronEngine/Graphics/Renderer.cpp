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

Renderer* Renderer::Get()
{
	static Renderer* instance;
	if (!instance)
		instance = new Renderer();
	return instance;
}

void Renderer::BindShader(rsc<Shader, false> shader)
{
	unsigned int bufferID = shader->getBufferID();
	if (m_bound_shader == bufferID)
		return;
	glUseProgram(bufferID);
	m_bound_shader = bufferID;
}

void Renderer::BindModel(rsc<Model, false> model)
{

	unsigned int bufferID = model->getBufferID();
	if (m_bound_shader == bufferID)
		return;
	model->getVertexArray()->Bind();
	model->getIndexBuffer()->Bind();
	m_bound_shader = bufferID;
}

void Renderer::BindMaterial(rsc<Material, false> material)
{
}

void Renderer::UnbindShader()
{
	glUseProgram(0);
	m_bound_shader = 0;
}

void Renderer::UnbindModel()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	m_bound_model = 0;
}

void Renderer::UnbindMaterial()
{
}


void Renderer::Clear(Vector4 color) const
{
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(rsc<Model, false> model, rsc<Shader, false> shader) const
{
	shader->Bind();

	VertexArray* va = model->getVertexArray();
	IndexBuffer* ib = model->getIndexBuffer();

	va->Bind();
	ib->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib->getCount(), GL_UNSIGNED_INT, nullptr));

	//Unbinding for debug; otherwise 

}

void Renderer::Draw(rsc<Shader, false> shader, rsc<Model, false> model, rsc<Material, false> material)
{

}