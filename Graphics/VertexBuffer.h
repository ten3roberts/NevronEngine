#pragma once
#include <src/pch.h>
class VertexBuffer
{
private:
	//Buffer id
	unsigned int m_bufferID;
public:
	//Creates and binds a vertex buffer
	VertexBuffer(const void* data, unsigned int size);
	
	~VertexBuffer();

	void setData(const void* data, unsigned int size);


	void Bind() const;
	void Unbind() const;

	unsigned int getBufferID() const { return m_bufferID; }
};



