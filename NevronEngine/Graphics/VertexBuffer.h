#pragma once
class VertexBuffer
{
private:
	//Buffer id
	unsigned int m_rendererID;
public:
	//Creates and binds a vertex buffer
	VertexBuffer(const void* data, unsigned int size);
	
	~VertexBuffer();

	void setData(const void* data, unsigned int size);


	void Bind() const;
	void Unbind() const;

	unsigned int getID() const { return m_rendererID; }
};



