#pragma once
#include <src/pch.h>

#include <src/Component.h>
#include <Graphics/Material.h>

#include <sstream>

struct ShaderSource
{
	ShaderSource(std::stringstream ss[2]) : vertexSource(ss[0].str()), fragmentSource(ss[1].str()) {};
	std::string vertexSource, fragmentSource;
	const std::string operator[](unsigned int index) const { return *(&vertexSource + index); };
};

class Shader : public Component
{
private:
	std::string m_filepath;
	//Uniform caching
	std::unordered_map<std::string, int> m_uniformCache;
	std::unordered_map<std::string, rsc<UniformBuffer>> m_uniformBuffers;
public:
	//Compilation from file
	Shader(const std::string& name);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//set uniforms

	//sets a quaternion uniform
	void setUniform4f(const std::string& name, Quaternion value);

	//sets a matrix4x4 uniform
	void setUniformMat4f(const std::string& name, Matrix4 value, bool transpose = false);

	//sets a vec4 uniform
	void setUniform4f(const std::string& name, Vector4 value);
	
	//sets a vec3 uniform
	void setUniform3f(const std::string& name, Vector3 value);
	
	//sets a vec2 uniform
	void setUniform2f(const std::string& name, Vector2 value);

	//sets a float uniform
	void setUniform1f(const std::string& name, float value);

	//sets an integer value
	void setUniform1i(const std::string& name, int value);

	//Sets a shader specific uniform buffer by name with the data provided
	bool setUniformBuffer(const std::string& name, void* data, size_t size, size_t offset = 0);

	//Returns a shader specific uniform buffer by name
	rsc<UniformBuffer> getUniformBuffer(const std::string name);

	void setMaterial(rsc_weak<Material> material);


	std::string getName() const { return m_name; }

private:
	int getUniformLocation(const std::string& name);

	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CreateShader(const ShaderSource& source);
	ShaderSource ParseShader(const std::string& path);
	void ProcessShader(const ShaderSource& source);
};



