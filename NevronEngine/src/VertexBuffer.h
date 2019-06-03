#pragma once
class VertexBuffer
{
private:
	//Buffer id
	unsigned int m_rendererID;
public:
	//Creates and binds a vertex buffer
	VertexBuffer(const void* data, unsigned int size);

	//Creates and binds a vertex buffer depening on $bind
	VertexBuffer(const void* data, unsigned int size, bool bind);

	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};

