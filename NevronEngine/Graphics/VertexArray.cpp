#include <src/pch.h>
#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_bufferID);
	glBindVertexArray(m_bufferID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_bufferID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	//Binds buffer
	vb.Bind();
	//Adds the layout to it
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	for(unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.Normalized, layout.getstride(), (const void*)offset);
		offset += element.count * VBElement::getSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_bufferID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}


