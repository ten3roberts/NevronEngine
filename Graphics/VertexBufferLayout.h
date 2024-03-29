#pragma once
#include <src/pch.h>
#include "GL/glew.h"

struct VBElement
{
	unsigned int type;
	unsigned int count;
	unsigned int Normalized;
	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}
		ASSERT(false);
		return 0;
	}
};
class VertexBufferLayout
{
private:
	std::vector<VBElement> m_elements;
	unsigned int m_stride;
public:
	VertexBufferLayout()
		: m_stride(0) {}

	template <typename T>
	void Push(unsigned int count)
	{
		
	}

	

	inline const std::vector<VBElement> getElements() const { return m_elements; }
	inline unsigned int getstride() const { return m_stride; }

};

	template <>	
	inline void VertexBufferLayout::Push<float>(unsigned int count)
	{
		m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_stride += count * VBElement::getSizeOfType(GL_FLOAT);
	}

	template <>
	inline void VertexBufferLayout::Push<unsigned int>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += count * VBElement::getSizeOfType(GL_UNSIGNED_INT);
	}

	template <>
	inline void VertexBufferLayout::Push<unsigned char>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_stride += count * VBElement::getSizeOfType(GL_UNSIGNED_BYTE);
	}
