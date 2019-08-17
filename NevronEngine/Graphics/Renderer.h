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
private:
	~Renderer();
	Renderer();
	unsigned int m_bound_shader;
	unsigned int m_bound_model;
	unsigned int m_bound_material;
public:
	static Renderer* Get();

	//Binds a shader and keeps if it's not already bound
	void BindShader(rsc<Shader, false> shader);

	//Binds a model and keeps if it's not already bound
	void BindModel(rsc<Model, false> model);

	//Binds a material to the current shader and keeps if it's not already bound
	void BindMaterial(rsc<Material, false> material);

	void UnbindShader();
	void UnbindModel();
	void UnbindMaterial();

	void Clear(Vector4 color = Vector4::black) const;
	void Draw(rsc<Model, false> model, rsc<Shader, false> shader) const;
	void Draw(rsc<Shader, false> shader, rsc<Model, false> model, rsc<Material, false> material);
};