#pragma once

#include "..\\Math\Vector.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include <vector>

struct Vertex
{
	Vertex() : position(0) {};
	Vertex(Vector3 pos, Vector2 uv) : position(pos), uv(uv) {}
	
	Vector3 position;
	Vector2 uv;
};

class Model
{
private:
	//std::vector<Vertex> m_vertices;
	//std::vector<unsigned int> m_indices;

	VertexBuffer *m_vertexBuffer;
	IndexBuffer *m_indexBuffer;
	VertexArray *m_vertexArray;
public:
	Model(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices);
	Model(Vertex* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount);
	~Model();

	VertexBuffer* getVertexBuffer() const { return m_vertexBuffer; }
	IndexBuffer* getIndexBuffer() const { return m_indexBuffer; }
	VertexArray* getVertexArray() const { return m_vertexArray; }
	void setVertices(std::vector<Vertex> vertices);
	void setIndices(std::vector<unsigned int> indices);

	void Bind() const;
	void Unbind() const;
};

