#include "UniformBuffer.h"
#include <Graphics/Shader.h>
#include <src/Logger.h>
#include "GL/glew.h"
#include <src/ResourceManager.h>

UniformBuffer::UniformBuffer(const std::string& name, const void* data, unsigned int size)
{
	m_name = name;
	//Buffer generation
	glGenBuffers(1, &m_bufferID);
	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferID);

	//Allocation
	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
	m_slot = ResourceManager::Get()->GetUBOSlot();
}

UniformBuffer::UniformBuffer(const std::string& name)
{
	m_name = name;

	//Buffer generation
	glGenBuffers(1, &m_bufferID);
	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferID);

	//Allocation
	glBufferData(GL_UNIFORM_BUFFER, 0, 0, GL_DYNAMIC_DRAW);
	m_slot = ResourceManager::Get()->GetUBOSlot();
}

UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &m_bufferID);
	ResourceManager::Get()->FreeUBOSlot(m_slot);
}

void UniformBuffer::setData(Shader* shader, const void* data, unsigned int size, unsigned int offset)
{
	//Obtain pointer of mapped data
	void* buffer = glMapBuffer(GL_UNIFORM_BUFFER, GL_READ_WRITE);

	if (buffer == nullptr)
	{
		LogS("Error", "Uniform buffer could not be mapped");
		return;
	}

	memcpy(buffer, (char*)data + offset, size);

	glUnmapBuffer(GL_UNIFORM_BUFFER);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_slot, m_bufferID);
	int uniformIndex = glGetUniformBlockIndex(shader->getBufferID(), "Environment");
	glUniformBlockBinding(shader->getBufferID(), uniformIndex, m_slot);
}

void UniformBuffer::setData(unsigned int shaderID, const void* data, unsigned int size, unsigned int offset)
{
	//Obtain pointer of mapped data
	void* buffer = glMapBuffer(GL_UNIFORM_BUFFER, GL_READ_WRITE);

	if (buffer == nullptr)
	{
		LogS("Error", "Uniform buffer could not be mapped");
		return;
	}

	memcpy(buffer, (char*)data + offset, size);

	glUnmapBuffer(GL_UNIFORM_BUFFER);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_slot, m_bufferID);
	int uniformIndex = glGetUniformBlockIndex(shaderID, "Environment");
	glUniformBlockBinding(shaderID, uniformIndex, m_slot);
}


