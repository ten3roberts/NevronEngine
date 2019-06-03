#pragma once
class IndexBuffer
{
private:
	unsigned int m_rendererID;
	unsigned int m_count;
public:

	//Creates and binds a index buffer depening on $bind
	IndexBuffer(unsigned int* data, unsigned int count);

	//Creates and binds a index buffer depening on $bind
	IndexBuffer(unsigned int* data, unsigned int count, bool bind);

	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	//Getters and setter
	inline unsigned int getCount() const { return m_count; }
};

