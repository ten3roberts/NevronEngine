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

Model::Model(const std::string& name) : m_filepath(""), m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_vertexArray(nullptr)
{
	//TODO: load model from collada file
	LoadDAE(name);
	if (m_vertexArray)
		m_bufferID = m_vertexArray->getBufferID();

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
	if (!dae.isValid())
	{
		LogS("Model : " + m_name, "Couldn't load file");
		return;
	}
	m_filepath = dae.getFilepath();
	XMLNode* mesh = dae.Find("mesh");

	std::string mesh_id = mesh->getParent()->getAttribute("id");
	std::vector<XMLNode*>& sources = *mesh->getChildren("source");

	size_t position_count = 0;
	Vector3* positions = nullptr;

	size_t uv_count = 0;
	Vector2* uvs = nullptr;

	size_t normal_count = 0;
	Vector3* normals = nullptr;
	//Correcting up axis
	void (*swap)(Vector3&) = [](Vector3& vec) {};
	std::string up_axis = dae.Find("up_axis")->getContent();

	
	if (up_axis == "Z_UP")
		swap = swapYZ;
	if (up_axis == "X_UP")
		swap = swapXY;
	for (int i = 0; i < sources.size(); i++)
	{
		std::string source_id = sources[i]->getAttribute("id");
		XMLNode& source = *sources[i];
		if (source_id == mesh_id + "-positions")
		{

			position_count = numi(source["float_array"].getAttribute("count")) / 3;
			positions = new Vector3[position_count];
			std::vector<std::string> arr = strSplit(source["float_array"].getContent(), ' ');

			for (size_t j = 0; j < arr.size() - 2; j += 3)
			{
				positions[j / 3] = Vector3(numf(arr[j]), numf(arr[j + 1]), numf(arr[j + 2]));
				swap(positions[j / 3]);
			}
		}
		else if (source_id == mesh_id + "-map-0")
		{
			uv_count = numi(source["float_array"].getAttribute("count")) / 2;
			uvs = new Vector2[uv_count];
			std::vector<std::string> arr = strSplit(source["float_array"].getContent(), ' ');

			for (size_t j = 0; j < arr.size(); j += 2)
				uvs[j / 2] = Vector2(numf(arr[j]), numf(arr[j + 1]));
		}
		else if (source_id == mesh_id + "-normals")
		{
			normal_count = numi(source["float_array"].getAttribute("count")) / 3;
			normals = new Vector3[normal_count];
			std::vector<std::string> arr = strSplit(source["float_array"].getContent(), ' ');

			for (size_t j = 0; j < arr.size() - 2; j += 3)
			{
				normals[j / 3] = Vector3(numf(arr[j]), numf(arr[j + 1]), numf(arr[j + 2]));
				swap(normals[j / 3]);
			}
		}
	}

	if (!positions)
	{
		LogS("Model : " + m_name, "Couldn't load positions from DAE");
		return;
	}
	if (!uvs)
	{
		LogS("Model : " + m_name, "Couldn't load uvs from DAE");
		return;
	}
	if (!normals)
	{
		LogS("Model : " + m_name, "Couldn't load normals from DAE");
		return;
	}

	//Getting the layout of the index data
	XMLNode& triangles = (*mesh)["triangles"];
	std::vector<XMLNode*>& inputs = *triangles.getChildren("input");

	size_t pos_offset = 0;
	size_t normal_offset = 0;
	size_t uv_offset = 0;

	for (size_t i = 0; i < inputs.size(); i++)
	{
		std::string semantic = inputs[i]->getAttribute("semantic");
		if (semantic == "VERTEX")
		{
			pos_offset = numi(inputs[i]->getAttribute("offset"));
		}
		else if (semantic == "NORMAL")
		{
			normal_offset = numi(inputs[i]->getAttribute("offset"));
		}
		else if (semantic == "TEXCOORD")
		{
			uv_offset = numi(inputs[i]->getAttribute("offset"));
		}
	}

	size_t index[3] = { 0,0,0 };

	int curr_pos, curr_normal, curr_uv;
	struct vert_triple
	{
		vert_triple(int posi, int normi, int uvi) : posi(posi), normi(normi), uvi(uvi) {}
		vert_triple(const vert_triple& other) : posi(other.posi), normi(other.normi), uvi(other.uvi) {}
		unsigned int posi, normi, uvi;
		bool operator==(const vert_triple& other) const { return posi == other.posi && normi == other.normi && uvi == other.uvi; }

	};
	struct vert_triple_hash
	{
		size_t operator()(const vert_triple& in) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			return ((hash<unsigned int>()(in.posi)
				^ (hash<unsigned int>()(in.normi) << 1)) >> 1)
				^ (hash<unsigned int>()(in.uvi) << 1);
		}
	};
	//A map linking position and uv data to the vertex array
	std::unordered_map<vert_triple, size_t, vert_triple_hash> vertex_map;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	std::vector<std::string> index_data = strSplit(triangles["p"].getContent(), ' ');
	for (size_t i = 0; i < index_data.size(); i += 3)
	{
		index[0] = numi(index_data[i]);
		index[1] = numi(index_data[i + 1]);
		index[2] = numi(index_data[i + 2]);

		curr_pos = index[pos_offset];
		curr_normal = index[normal_offset];
		curr_uv = index[uv_offset];

		auto it = vertex_map.find({ curr_pos, curr_normal, curr_uv });

		//if the vertex with specified position and uv doesn't exist
		if (it == vertex_map.end())
		{
			vertices.push_back(Vertex(positions[curr_pos], uvs[curr_uv]));
			vertex_map.insert(std::pair<vert_triple, int>({ curr_pos, curr_normal, curr_uv }, vertices.size() - 1));
			indices.push_back(vertices.size() - 1);
		}
		//The vertex has already been created
		else
		{
			indices.push_back(it->second);
		}
	}

	m_vertexBuffer = new VertexBuffer(&(vertices[0]), sizeof(Vertex) * vertices.size());
	m_indexBuffer = new IndexBuffer(&(indices[0]), indices.size());
	m_vertexArray = new VertexArray();

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);

	//Adds vertices
	m_vertexArray->AddBuffer(*m_vertexBuffer, layout);

	delete[] positions;
	delete[] uvs;
	delete[] normals;
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


