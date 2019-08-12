#pragma once
#include <src/Systemdefs.h>
#include <Graphics/Model.h>
#include <Graphics/Shader.h>

#include <GL/glew.h>
#include <string>

#pragma region "GL Error Handling"

#define GLCall(x) GLClearError();	x;	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLAPIENTRY ErrorCallBack(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);


//GL Error handling
#pragma endregion
class Renderer
{
public:
	Renderer();
	~Renderer();

	void Clear(Vector4 color = Vector4::black) const;
	void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const;
	void Draw(Model* model, const Shader& shader) const;
};


