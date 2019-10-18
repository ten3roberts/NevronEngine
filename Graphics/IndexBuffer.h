#pragma once
#include <src/pch.h>
class IndexBuffer
{
private:
	unsigned int m_bufferID;
	unsigned int m_count;
public:

	//Creates and binds a index buffer depening on $bind
	IndexBuffer(unsigned int* data, unsigned int count);

	~IndexBuffer();

	void setData(unsigned int* data, unsigned int count);

	void Bind() const;
	void Unbind() const;

	//getters and setter
	inline unsigned int getCount() const { return m_count; }
};



