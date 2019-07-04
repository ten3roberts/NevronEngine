#pragma once
class IndexBuffer
{
private:
	unsigned int m_rendererID;
	unsigned int m_count;
public:

	//Creates and binds a index buffer
	IndexBuffer(unsigned int* data, unsigned int count);

	void setData(unsigned int* data, unsigned int count);

	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	//getters and setter
	inline unsigned int getCount() const { return m_count; }
};

