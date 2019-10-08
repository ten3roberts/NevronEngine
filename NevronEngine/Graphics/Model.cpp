#include <src/pch.h>
#include "Model.h"
#include "Renderer.h"

using namespace Utility;

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
	//TODO: load model from collada file
	LoadDAE(name);
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

void Model::LoadDAE(const std::string& filename)
{
	m_name = filename;

	XMLNode dae(filename);
	m_filepath = dae.getFilepath();
	XMLNode* mesh = dae.Find("mesh");


	std::string mesh_id = mesh->getParent()->getAttribute("id");
	std::vector<XMLNode*>& sources = *mesh->getChildren("source");

	size_t position_count = 0;
	Vector3* positions = nullptr;

	size_t index_count = 0;
	unsigned int* indices = nullptr;

	size_t normal_count = 0;
	Vector3* normals = nullptr;

	for (int i = 0; i < sources.size(); i++)
	{
		std::string source_id = sources[i]->getAttribute("id");
		if (source_id == mesh_id + "-positions")
		{
			
			position_count = numi(sources[i]["float_array"].getAttribute("count"));
			positions = new Vector3[position_count];
			std::vector<std::string> arr = strSplit(sources[i]["float_array"].getContent(), ' ');

			for (size_t j = 0; j < arr.size() - 2; j += 3)
				positions[j/3] = Vector3(numf(arr[j]), numf(arr[j + 1]), numf(arr[j + 2]));



		}
		if (source_id == mesh_id + "-map-0-array")
		{
			index_count = numi(sources[i]->getAttribute("count"));
			indices = new unsigned int[index_count];
			std::vector<std::string> arr = strSplit((*sources[i])["float_array"]->getContent(), ' ');

			for (size_t j = 0; j < arr.size(); j++)
				positions[j] = numi(arr[j]);
		}
		if (source_id == mesh_id + "-normals")
		{
			normal_count = numi(sources[i]->getAttribute("count"));
			normals = new Vector3[normal_count];
			std::vector<std::string> arr = strSplit((*sources[i])["float_array"]->getContent(), ' ');

			for (size_t j = 0; j < arr.size() - 2; j += 3)
				normals[j / 3] = Vector3(numf(arr[j]), numf(arr[j + 1]), numf(arr[j + 2]));


		}
	}
	LogF("%s", normals[2].str());
	/*
	XMLNode* triangles = mesh->operator[]("triangles");
	std::vector<XMLNode>& inputs = triangles->getChildren()["input"];


	for(int i = 0; i < inputs.size(); i++)
	{
		std::string semantic = inputs[i].getAttribute("SEMANTIC");
		if (semantic == "VERTEX")
		{

		}
	}*/
	if (positions)
		delete [] positions;
	if (indices)
		delete [] indices;
	if (normals)
		delete [] normals;
}

Model* Model::GenerateQuad(Vector2 size)
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


	return new Model(vertices, 4, indices, 6);
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
	Renderer::Get()->BindModel(this);
}

void Model::Unbind() const
{
	Renderer::Get()->UnbindModel();
}

void Model::ApplyLayout(VertexBufferLayout* layout)
{
	layout->Push<float>(3);
	layout->Push<float>(2);
}


