#pragma once

#include "..\\Math\Vector.h"
#include "VertexBuffer.h"
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
	Model();
	Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	~Model();

	std::vector<Vertex>& getVertices() { return m_vertices; }
	std::vector<unsigned int>& getIndices() { return m_indices; }
	VertexBuffer& getVertexBuffer() { return m_vertexBuffer; }
	void setVertices(std::vector<Vertex> vertices);
	void setIndices(std::vector<unsigned int> indices);
};

