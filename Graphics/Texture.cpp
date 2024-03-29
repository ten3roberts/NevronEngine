#include <src/pch.h>
#include "Texture.h"
#include <Graphics/Renderer.h>

#define STB_IMAGE_IMPLEMENTATION
#include <src/vendor/stb_image/stb_image.h>

#include <GL/glew.h>
using namespace Utility;

Texture::Texture(const std::string& name, unsigned int slot, bool flip)
	: m_slot(slot), m_localBuffer(nullptr), m_width(0), m_height(0), m_BPP(0)
{
	m_name = getFilename(name, true);
	m_filepath = FindFile(m_name);

	if (!m_filepath.size())
	{
		LogS("Texture : " + m_name, "Couldn't find texture with name: ");
		m_valid = false;
		return;
	}

	LogS("Texture: " + m_name, "Loading texture: %s", ShortenPath(m_filepath));

	stbi_set_flip_vertically_on_load(flip);
	m_localBuffer = stbi_load(m_filepath.c_str(), &m_width, &m_height, &m_BPP, 4);

	glGenTextures(1, &m_bufferID);
	glBindTexture(GL_TEXTURE_2D, m_bufferID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);

	glBindTexture(GL_TEXTURE_2D, 0);

	/*if (m_localBuffer)
		stbi_image_free(m_localBuffer);*/
}

Texture::Texture(const std::string& name)
	: m_slot(0), m_localBuffer(nullptr), m_width(0), m_height(0), m_BPP(0)
{
	m_name = strLead(getFilename(name, true), ".png");
	m_filepath = FindFile(m_name);
	LogS("Texture: " + m_name, "Loading texture: %s", ShortenPath(m_filepath));

	stbi_set_flip_vertically_on_load(true);
	m_localBuffer = stbi_load(m_filepath.c_str(), &m_width, &m_height, &m_BPP, 4);

	glGenTextures(1, &m_bufferID);
	glBindTexture(GL_TEXTURE_2D, m_bufferID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);

	glBindTexture(GL_TEXTURE_2D, 0);

	/*if (m_localBuffer)
		stbi_image_free(m_localBuffer);*/
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_bufferID);
	if (m_localBuffer)
		stbi_image_free(m_localBuffer);
}

void Texture::Bind() const
{
	Renderer::Get()->BindTexture(this);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}


