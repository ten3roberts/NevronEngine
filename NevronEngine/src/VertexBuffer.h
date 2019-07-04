#pragma once
class VertexBuffer
{
private:
	//Buffer id
	unsigned int m_rendererID;
public:
	//Creates and binds a vertex buffer
	VertexBuffer(const void* data, unsigned int size);

	void setData(const void* data, unsigned int size);

	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};

