#include "UniformBuffer.h"
#include <Graphics/Shader.h>
#include <src/Logger.h>
#include "GL/glew.h"
#include <src/ResourceManager.h>

UniformBuffer::UniformBuffer(const std::string& name, const void* data, size_t size, unsigned int slot) : m_size(size), m_slot(slot)
{
	m_name = name;
	//Buffer generation
	glGenBuffers(1, &m_bufferID);
	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferID);

	//Allocation
	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
	//m_slot = ResourceManager::Get()->GetUBOSlot();
}

UniformBuffer::UniformBuffer(const std::string& name)
{
	/*m_name = name;

	//Buffer generation
	glGenBuffers(1, &m_bufferID);
	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferID);

	//Allocation
	glBufferData(GL_UNIFORM_BUFFER, 0, 0, GL_DYNAMIC_DRAW);*/
}

UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &m_bufferID);
}

void UniformBuffer::setData(Shader* shader, const void* data, size_t size, size_t offset)
{
	setData(shader->getBufferID(), data, size, offset);
}

void UniformBuffer::setData(unsigned int shaderID, const void* data, size_t size, size_t offset)
{
	//Check if size is too large
	if (size == 0) size = m_size;
	if (size > m_size)
	{
		LogS("UniformBuffer : " + m_name, "Insufficient buffer size. Allocated size %t, requested size %t", m_size, size);
		return;
	}

	//Obtain pointer of mapped data
	void* buffer = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);

	if (buffer == nullptr)
	{
		LogS("UnformBuffer " + m_name, "Uniform buffer could not be mapped for shader %d", shaderID);
		return;
	}

	memcpy(buffer, (char*)data + offset, size);

	glUnmapBuffer(GL_UNIFORM_BUFFER);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_slot, m_bufferID);
	int uniformIndex = glGetUniformBlockIndex(shaderID, "Environment");
	glUniformBlockBinding(shaderID, uniformIndex, m_slot);
}


