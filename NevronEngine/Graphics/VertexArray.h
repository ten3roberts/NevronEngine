#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

//Vertex Attribute Array
class VertexArray
{
private:
	//Vertex attrib object; vao
	unsigned int m_rendererID;
public:
	VertexArray();
	~VertexArray();

	//Adds a vertex buffer, i.e; a model and assigns the vertex layout
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;

	unsigned int getID() const { return m_rendererID; }


};

