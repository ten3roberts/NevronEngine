#include <src/pch.h>
#include "Shader.h"
#include "..\src\Utility.h"
#include "Renderer.h"
#include "..\src\Logger.h"
#include <src/ResourceManager.h>
#include <Graphics/UniformBuffer.h>

using namespace Utility;

Shader::Shader(const std::string& name)
{
	m_name = getFilename(name, true);
	m_filepath = FindFile(m_name, ".glsl");


	if (!m_filepath.size())
	{
		LogS("Shader : " + m_name, "Couldn't find shader with name: ");
		m_valid = false;
		return;
	}

	ShaderSource source = ParseShader(m_filepath);
	ProcessShader(source);
	m_bufferID = CreateShader(source);
}

Shader::~Shader()
{
	glDeleteProgram(m_bufferID);
	LogS("Shader : " + m_name, "Destructor called");
}

void Shader::Bind() const
{
	Renderer::Get()->BindShader(this);
}

void Shader::Unbind() const
{
	Renderer::Get()->UnbindShader();
}

#pragma region setUniform
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

void Shader::setUniform1i(const std::string& name, int value)
{
	glUniform1i(getUniformLocation(name), value);
}
#pragma endregion

bool Shader::setUniformBuffer(const std::string& name, void* data, size_t size, size_t offset)
{
	auto ubo = m_uniformBuffers.find(name);
	if (ubo == m_uniformBuffers.end())
	{
		LogS("Shader : " + m_name, "Shader contains no uniform buffer with name %s", name);
		return false;
	}
	ubo->second->setData(m_bufferID, data, size, offset);
	return true;
}

rsc<UniformBuffer> Shader::getUniformBuffer(const std::string name)
{
	return m_uniformBuffers[name];
}

void Shader::setMaterial(rsc_weak<Material> material)
{
	//rsc<UniformBuffer> materialBuffer = ResourceManager::Get()->GetUBO("material" + (material->getSlot() > 0 ? std::to_string(material->getSlot()) : ""));
	//materialBuffer->setData(this, &material->generateBuffer(), sizeof(MaterialType));
	//Bind();
	//m_uniformBuffers["Material"]->setData(this, &material->generateBuffer(), sizeof(MaterialType));

	if (material->getTexture())
		glUniform1i(getUniformLocation("u_texture"), material->getTexture()->getSlot()); //Texture

	if (material->getNormalMap())
		glUniform1i(getUniformLocation("u_normalMap"), material->getNormalMap()->getSlot()); //Texture1

	if (material->getSpecularMap())
		glUniform1i(getUniformLocation("u_specularMap"), material->getSpecularMap()->getSlot()); //Texture2


	glUniform4f(getUniformLocation("u_color"), material->color.b, material->color.g, material->color.b, material->color.a); //Color
}

int Shader::getUniformLocation(const std::string& name)
{
	if (m_uniformCache.find(name) != m_uniformCache.end())
		return m_uniformCache[name];

	//LogS("Shader: " + m_name, "Uniform: %s %s", name, "is not yet cached, retrieving id");
	int location = glGetUniformLocation(m_bufferID, name.c_str());
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
		m_valid = false;
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

unsigned int Shader::CreateShader(const ShaderSource& source)
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
	ShaderSource source(ss);

	if (source.vertexSource.size() == 0)
		LogS("Could not parse vertex shader", "");
	if (source.fragmentSource.size() == 0)
		LogS("Could not parse fragment shader", "");
	return source;
}

void Shader::ProcessShader(const ShaderSource& source)
{
	std::unordered_map<std::string, size_t> types;
	types["bool"] = 4;
	types["int"] = 4;
	types["sampler2D"] = 4;
	types["float"] = 4;

	types["bvec2"] = 8;
	types["bvec3"] = 12;
	types["bvec4"] = 16;

	types["vec2"] = 8;
	types["vec3"] = 12;
	types["vec4"] = 16;

	types["mat3"] = 36;
	types["mat4"] = 64;

	std::string curr_line;
	unsigned int pos = 0;

	std::string complete_source;


	for (unsigned int i = 0; i < 2; i++)
	{
		bool single_comment = false;
		bool multi_comment = false;
		for (size_t j = 0; j < source[i].size(); j++)
		{
			if (source[i].substr(j, 2) == "//") //Single line comment starts
				single_comment = true;
			if (source[i][j] == '\n') //Single line comment ends
				single_comment = false;

			if (source[i].substr(j, 2) == "/*") //Multiline comment starts
				multi_comment = true;
			if (source[i].substr(j, 2) == "*/") //Multiline comment ends
			{
				multi_comment = false;
				j += 2;
			}


			if (!single_comment && !multi_comment)
				complete_source += source[i][j];
		}
	}

	//Splits the source into semicolon statements
	std::vector<std::string> lines = strSplit(complete_source, ';'); //In terms of semicolons

	enum BufferType
	{
		Struct, UBO, None
	};

	BufferType buffer_type = None;
	const char* names[2] = { "struct ", "uniform " };

	for (size_t i = 0; i < lines.size() - 1; i++)
	{
		size_t definition_pos = lines[i].find(names[Struct]);

		buffer_type = Struct;
		if (definition_pos == std::string::npos)
		{
			definition_pos = lines[i].find(names[UBO]);
			buffer_type = UBO;
		}

		if (definition_pos == std::string::npos)
			continue;

		std::string tmp = lines[i].substr(definition_pos + strlen(names[buffer_type]));
		size_t open_bracket = tmp.find('{');

		if (open_bracket == std::string::npos) //It is a buffer and not a single uniform
			continue;

		std::string name = tmp.substr(0, open_bracket);
		tmp = tmp.substr(open_bracket + 1);

		//The structs/uniformbuffer's name
		name = strPurgeAll(name, "\n ");
		std::vector<std::string> members;
		size_t buffer_size = 0;


		//Adding all members
		tmp = tmp.substr(tmp.find_first_not_of("\n\t "));
		members.push_back(strPurgeAll(tmp.substr(0, tmp.find(' ')), "\n\t "));
		buffer_size += types[strPurgeAll(tmp.substr(0, tmp.find(' ')), "\n\t ")];
		for (size_t j = i + 1; j < lines.size(); j++)
		{
			//The uniform buffer declaration has ended
			if (lines[j].find('}') != std::string::npos)
				break;

			lines[j] = lines[j].substr(lines[j].find_first_not_of("\n\t "));

			members.push_back(strPurgeAll(lines[j].substr(0, lines[j].find(' ')), "\n\t "));

			std::string member_name = strPurgeAll(lines[j].substr(lines[j].find(' ')), "\n\t ");

			size_t arr_size = 1;
			size_t arr_start = member_name.find_first_of('[');
			if (arr_start != std::string::npos)
			{
				std::string arr = member_name.substr(arr_start);
				arr = arr.substr(1, arr.find(']') - 1);
				if (arr.size()) //Unknown size of array
				{

					arr_size = atoi(arr.c_str());
				}
			}

			buffer_size += types[strPurgeAll(lines[j].substr(0, lines[j].find(' ')), "\n\t ")] * arr_size;
		}

		//Adds the struct as a data type
		if (buffer_type == Struct)
		{
			if (types.find(name) == types.end())
				types[name] = buffer_size;
			else
				LogS("Shader : " + m_name, "Multiple definition of struct %s", name);
			return;
		}
		if (buffer_size % 4 != 0)
		{
			LogS("Shader : " + m_name, "Uniform buffer is not divisive with 4, size %t", buffer_size);
		}

		if (m_uniformBuffers.find(name) == m_uniformBuffers.end())
			m_uniformBuffers[name] = new UniformBuffer(name, nullptr, buffer_size, m_uniformBuffers.size());
		else
		{
			LogS("Shader : " + m_name, "Multiple definition of buffer %s", name);
		}

		LogS("Shader : " + m_name, "Uniform buffer : %s, size : %t", name, buffer_size);

	}
}

