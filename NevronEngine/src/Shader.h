#pragma once
#include <string>
#include "..\\Math\Math.h"

#include <sstream>
#include <unordered_map>

struct ShaderSource
{
	ShaderSource(std::stringstream ss[2]) : vertexSource(ss[0].str()), fragmentSource(ss[1].str()) {};
	std::string vertexSource, fragmentSource;
	std::string operator[](unsigned int index) { return *(&vertexSource + index); };
};

class Shader
{
private:
	std::string m_filepath;
	std::string m_name;
	unsigned int m_rendererID;
	//Uniform caching
	std::unordered_map<std::string, int> m_uniformCache;
public:
	//Compilation from file
	Shader(const std::string& name);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//Set uniforms

	//Sets a quaternion uniform
	void SetUniform(const std::string& name, Quaternion value);

	//Sets a matrix4x4 uniform
	void SetUniform(const std::string& name, Matrix4 value);

	//Sets a vec4 uniform
	void SetUniform(const std::string& name, Vector4 value);
	
	//Sets a vec2 uniform
	void SetUniform(const std::string& name, Vector2 value);

	//Sets a float uniform
	void SetUniform(const std::string& name, float value);


	std::string getName() const { return m_name; }

private:
	int getUniformLocation(const std::string& name);

	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CreateShader(ShaderSource source);
	ShaderSource ParseShader(const std::string& path);
};

