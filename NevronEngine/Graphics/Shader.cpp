#include "Shader.h"
#include "..\src\Utility.h"
#include "Renderer.h"
#include "..\src\Logger.h"

using namespace Utility;

Shader::Shader(const std::string& name)
{
	m_name = getFilename(name, false);
	m_filepath = FindFile(strLead(m_name, ".glsl"));
	if (m_filepath == "")
		LogS("Shader: " + m_name, "Couldn't find shader with name: ");

	ShaderSource source = ParseShader(m_filepath);
	m_rscID = CreateShader(source);
}

Shader::~Shader()
{
	glDeleteProgram(m_rscID);
}

void Shader::Bind() const
{
	glUseProgram(m_rscID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

//Shader needs to be bound before calling!
void Shader::setUniform4f(const std::string& name, Quaternion value)
{
	glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
}

//Shader needs to be bound before calling!
void Shader::setUniformMat4f(const std::string& name, Matrix4 value, bool transpose)
{
	glUniformMatrix4fv(getUniformLocation(name), 1, transpose, value[0]);
}

//Shader needs to be bound before calling!
void Shader::setUniform4f(const std::string& name, Vector4 value)
{

	glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Shader::setUniform3f(const std::string& name, Vector3 value)
{
	glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
}

//Shader needs to be bound before calling!
void Shader::setUniform2f(const std::string& name, Vector2 value)
{
	glUniform2f(getUniformLocation(name), value.x, value.y);
}

//Shader needs to be bound before calling!
void Shader::setUniform1f(const std::string& name, float value)
{
	glUniform1f(getUniformLocation(name), value);
}

void Shader::setUniform1i(const std::string name, int value)
{
	glUniform1i(getUniformLocation(name), value);
}

void Shader::setMaterial(Material* material)
{
	glUniform1i(getUniformLocation("u_texture"), material->texture->getSlot()); //Texture

	if (material->texture1)
		glUniform1i(getUniformLocation("u_texture1"), material->texture1->getSlot()); //Texture1
	if (material->texture2)
		glUniform1i(getUniformLocation("u_texture2"), material->texture2->getSlot()); //Texture2

	if (material->texture3)
		glUniform1i(getUniformLocation("u_texture3"), material->texture3->getSlot()); //Texture2

	glUniform4f(getUniformLocation("u_color"), material->color.b, material->color.g, material->color.b, material->color.a); //Color
}

int Shader::getUniformLocation(const std::string& name)
{
	if (m_uniformCache.find(name) != m_uniformCache.end())
		return m_uniformCache[name];

	//LogS("Shader: " + m_name, "Uniform: %s %s", name, "is not yet cached, retrieving id");
	int location = glGetUniformLocation(m_rscID, name.c_str());
	if (location == -1)
	{
	}	//LogS("Shader: " + m_name, "Couldn't get uniform location: %s%s", name, "; Uniform is either optimised away or does not exist in the current shader");
	else
		m_uniformCache[name] = location;
	return location;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* shaderSource = source.c_str();
	glShaderSource(id, 1, &shaderSource, nullptr);
	glCompileShader(id);

	//Error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	//There was a compile error
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char* message = new char[length * sizeof(char)];
		glGetShaderInfoLog(id, length, &length, message);
		LogS((type == GL_VERTEX_SHADER ? "vertex" : "fragment" + std::string(" Shader Error")), message);
		glDeleteShader(id);
		delete[] message;
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	LogS("Shader: " + m_name, "Compiled vertex shader");

	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	LogS("Shader: " + m_name, "Compiled fragment shader");

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	//Delete inscalarediates
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

unsigned int Shader::CreateShader(ShaderSource source)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, source.vertexSource);
	LogS("Shader: " + m_name, "Compiled vertex shader");

	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, source.fragmentSource);
	LogS("Shader: " + m_name, "Compiled fragment shader");

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	//Delete inscalarediates
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

ShaderSource Shader::ParseShader(const std::string& path)
{
	std::string filepath = path;

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};


	std::ifstream stream(filepath);
	std::string line;

	ShaderType type = ShaderType::NONE;
	std::stringstream ss[2];
	while (getline(stream, line))
	{
		//set mode to vertex shader
		if (line.find("#VERTEX_SHADER") != std::string::npos)
			type = ShaderType::VERTEX;

		//set mode to vertex shader
		else if (line.find("#FRAGMENT_SHADER") != std::string::npos)
			type = ShaderType::FRAGMENT;
		else
		{
			ss[(int)type] << line << "\n";
		}
	}
	ShaderSource sources(ss);

	if (sources.vertexSource.size() == 0)
		Error(1);
	if (sources.fragmentSource.size() == 0)
		Error(2);

	return sources;
}

