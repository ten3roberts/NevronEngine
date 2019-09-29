#pragma once
#include <src/pch.h>

#include <src/Component.h>
class Shader;

//Creates and manages a uniform buffer object. You can either create structs holding a few variables that it should set if it's matched up in the GPU
//You can also take part of a class or struct if only a limited number of members are to be set. You do this by changing size and making sure the variables wanted are inline
//After creation the UniformBuffer is only locked by size and not by object. This means that you can pass different objects to it as long as they have the same size and layout

class UniformBuffer : public Component
{
private:
	//The slot is to be determined by the ResourceManager and constant during lifetime
	unsigned int m_slot;
	size_t m_size;
public:
	//Creates and allocates a new uniform buffer to the currently bound shader. name is the GPU side buffer name, if no data is to be set pas NULL
	UniformBuffer(const std::string& name, const void* data, size_t size, unsigned int slot);
	UniformBuffer(const std::string& name);
	~UniformBuffer();

	//Will set the data of the uniform buffer in the given shader. Offset is in bytes. If size is set to zero whole buffer is assumed to be set
	void setData(Shader* shader, const void* data, size_t size, size_t offset = 0);
	void setData(unsigned int shaderID, const void* data, size_t size, size_t offset = 0);

	unsigned int getBufferID() const { return m_bufferID; }
	std::string getName() const { return m_name; }

	unsigned int getSlot() { return m_slot; }
	size_t getSize() { return m_size; }
};

