#include "Shader.h"
#include "Utility.h"
#include "Renderer.h"
#include "Logger.h"

using namespace Utility;

Shader::Shader(const std::string& name) : m_rendererID(0)
{
	m_name = getFilename(name);
	m_filepath = FindFile(strLead(name, ".shader"));
	if (m_filepath == "")
		logger << author << "Shader: " + m_name << "Couldn't find shader with name: " << lend;

	ShaderSource source = ParseShader(m_filepath);
	m_rendererID = CreateShader(source);
}

Shader::~Shader()
{
	glDeleteProgram(m_rendererID);
}

void Shader::Bind() const
{
	glUseProgram(m_rendererID);
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

int Shader::getUniformLocation(const std::string& name)
{
	if (m_uniformCache.find(name) != m_uniformCache.end())
		return m_uniformCache[name];

	logger << author << "Shader: " + m_name << "Uniform: " + name + " is not yet cached, retrieving id" << lend;
	int location = glGetUniformLocation(m_rendererID, name.c_str());
	if (location == -1)
		logger << author << "Shader: " + m_name << "Couldn't get uniform location: " + name + "; Uniform is either optimised away or does not exist in the current shader" << lend;
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
		logger << author << (type == GL_VERTEX_SHADER ? "vertex" : "fragment" + std::string(" Shader Error")) << message;
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
	logger << author << "Shader: " + m_name << "Compiled vertex shader" << lend;

	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	logger << author << "Shader: " + m_name << "Compiled fragment shader" << lend;

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
	logger << author << "Shader: " + m_name << "Compiled vertex shader" << lend;

	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, source.fragmentSource);
	logger << author << "Shader: " + m_name << "Compiled fragment shader" << lend;

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