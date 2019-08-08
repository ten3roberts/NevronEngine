#pragma once
#include <string>
#include <src/Component.h>
class Shader;

//Creates and manages a uniform buffer object. You can either create structs holding a few variables that it should set if it's matched up in the GPU
//You can also take part of a class or struct if only a limited number of members are to be set. You do this by changing size and making sure the variables wanted are inline
//After creation the UniformBuffer is only locked by size and not by object. This means that you can pass different objects to it as long as they have the same size and layout
class UniformBuffer
{
private:
	unsigned int m_bufferID;

	//The slot is to be determined by the ResourceManager and constant during lifetime
	unsigned int m_slot;

	//The name of the uniform buffer on the GPU side
	std::string m_name;
public:
	//Creates and allocates a new uniform buffer to the currently bound shader. name is the GPU side buffer name, if no data is to be set pas NULL
	UniformBuffer(const std::string& name, const void* data, unsigned int size);
	UniformBuffer(const std::string& name);
	~UniformBuffer();

	//Will set the data of the uniform buffer in the given shader. Offset is in bytes
	void setData(Shader* shader, const void* data, unsigned int size, unsigned int offset = 0);
	void setData(unsigned int shaderID, const void* data, unsigned int size, unsigned int offset = 0);

	unsigned int getID() const { return m_bufferID; }
	std::string getName() const { return m_name; }

	unsigned int getSlot() { return m_slot; }
};