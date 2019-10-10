#include <src/pch.h>
#include "Renderer.h"
#include "..\src\Utility.h"
#include <src/ResourceManager.h>
using namespace Utility;

void GLAPIENTRY ErrorCallBack(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	{
		if (type == 33361)
			return;
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

void Renderer::BindShader(const Shader* shader)
{
	unsigned int bufferID = shader->getBufferID();
	if (m_bound_shader == bufferID)
		return;
	//LogS("Renderer", "Binding shader %s", shader->getName());
	glUseProgram(bufferID);
	m_bound_shader = bufferID;
}

void Renderer::BindModel(const Model* model)
{
	unsigned int bufferID = model->getBufferID();
	if (m_bound_model == bufferID)
		return;
	//LogS("Renderer", "Binding model %s", model->getName());

	model->getVertexArray()->Bind();
	model->getIndexBuffer()->Bind();
	m_bound_model = bufferID;
}

void Renderer::BindMaterial(const Material* material)
{
}

void Renderer::BindTexture(const Texture* texture)
{
	unsigned int bufferID = texture->getBufferID();
	unsigned int slot = texture->getSlot();
	if (m_bound_textures[slot] == bufferID)
		return;

	//LogS("Renderer", "Binding texture %s", texture->getName());
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, bufferID);
	m_bound_textures[slot] = bufferID;
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

void Renderer::Draw(rsc_weak<Model> model, rsc_weak<Shader> shader) const
{
	shader->Bind();

	VertexArray* va = model->getVertexArray();
	IndexBuffer* ib = model->getIndexBuffer();

	va->Bind();
	ib->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib->getCount(), GL_UNSIGNED_INT, nullptr));

	//Unbinding for debug; otherwise 
}

void Renderer::Draw(rsc_weak<Shader> shader, rsc_weak<Model> model, rsc_weak<Material> material)
{
	shader->Bind();
	model->Bind();
	material->Bind();
	shader->setMaterial(material);

	VertexArray* va = model->getVertexArray();
	IndexBuffer* ib = model->getIndexBuffer();

	GLCall(glDrawElements(GL_TRIANGLES, ib->getCount(), GL_UNSIGNED_INT, nullptr));
}