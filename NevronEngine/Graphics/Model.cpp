#include "Model.h"

Model::Model(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices)
{
	m_vertexBuffer = new VertexBuffer(&(*vertices)[0], sizeof(Vertex) * vertices->size());
	m_indexBuffer = new IndexBuffer(&(*indices)[0], indices->size());
	m_vertexArray = new VertexArray();

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);

	//Adds vertices
	m_vertexArray->AddBuffer(*m_vertexBuffer, layout);
	m_bufferID = m_vertexArray->getBufferID();
}

Model::Model(const std::string& name)
{
	//TODO: load model from obj file
}

Model::Model(Vertex* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount)
{
	m_vertexBuffer = new VertexBuffer(vertices, sizeof(Vertex) * vertexCount);
	m_indexBuffer = new IndexBuffer(indices, indexCount);
	m_vertexArray = new VertexArray();

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);

	//Adds vertices
	m_vertexArray->AddBuffer(*m_vertexBuffer, layout);
}

Model::~Model()
{
	delete m_vertexBuffer;
	delete m_indexBuffer;
	delete m_vertexArray;
}

Model Model::GenerateQuad(Vector2 size)
{
	//Simple square
	Vertex vertices[4];
	vertices[0] = Vertex({ -0.5f * size.x, -0.5f * size.y, 0.0f }, { 0.0f, 0.0f });
	vertices[1] = Vertex({ 0.5f * size.x, -0.5f * size.y, 0.0f }, { 1.0f, 0.0f });
	vertices[2] = Vertex({ 0.5f * size.x, 0.5f * size.y, 0.0f }, { 1.0f, 1.0f });
	vertices[3] = Vertex({ -0.5f * size.x, 0.5f * size.y, 0.0f }, { 0.0f, 1.0f });

	unsigned int indices[6] = {
		0, 1, 2,
		2, 3, 0
	};


	return Model(vertices, 4, indices, 6);
}

void Model::setVertices(std::vector<Vertex> vertices)
{
	m_vertexBuffer->setData(&vertices[0], sizeof(Vertex) * vertices.size());
}

void Model::setIndices(std::vector<unsigned int> indices)
{
	m_indexBuffer->setData(&indices[0], sizeof(unsigned int) * indices.size());
}

void Model::Bind() const
{
	m_vertexArray->Bind();
	//m_vertexBuffer->Bind();
	m_indexBuffer->Bind();
}

void Model::Unbind() const
{
	m_vertexArray->Unbind();
	m_indexBuffer->Unbind();
}

void Model::ApplyLayout(VertexBufferLayout* layout)
{
	layout->Push<float>(3);
	layout->Push<float>(2);
}


