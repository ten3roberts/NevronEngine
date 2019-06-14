#pragma once
#include <string>

class Texture
{
private:
	unsigned int m_rendererID;
	std::string m_name;
	std::string m_filepath;
	unsigned char* m_localBuffer;
	int m_width, m_height, m_BPP;
public:
	Texture(const std::string& name, bool flip = true);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int getWidth() const { return m_width; }
	inline int getHeight() const { return m_height; }
	unsigned char* getData() { return m_localBuffer; };
};

