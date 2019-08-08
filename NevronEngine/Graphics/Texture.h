#pragma once
#include <src/Component.h>
#include <string>

class Texture : public Component
{
private:
	unsigned int m_slot;
	std::string m_filepath;
	unsigned char* m_localBuffer;
	int m_width, m_height, m_BPP;
public:
	Texture(const std::string& name, unsigned int slot = 0);
	~Texture();

	void Bind() const;
	void Unbind() const;

	inline int getWidth() const { return m_width; }
	inline int getHeight() const { return m_height; }
	unsigned char* getData() { return m_localBuffer; };

	unsigned int getSlot() { return m_slot; }
	void setSlot(unsigned int slot) { m_slot = slot; }
};



