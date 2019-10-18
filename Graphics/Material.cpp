#include <src/pch.h>
#include "Material.h"

#include <src/ResourceManager.h>

using namespace Utility;

Material::Material(const std::string& name) : m_slot(0), m_texture(nullptr), m_normalMap(nullptr), m_specularMap(nullptr)
{
	m_name = getFilename(name, true);

	m_filepath = FindFile(m_name);
	std::ifstream file(m_filepath);

	if (!m_filepath.size())
	{
		LogS("Shader : " + m_name, "Couldn't find shader with name: ");
		m_valid = false;
		return;
	}


	std::string line;
	while (file.good())
	{
		std::getline(file, line);

		//Texture
		if (line.find("texture: ") == 0)
		{
			m_texture = new Texture(line.substr(strlen("texture: "))); // ResourceManager::Get()->GetTexture(line.substr(strlen("texture: ")));
			if (m_texture)
				m_texture->setSlot(m_slot * 3);
		}

		//normalMap
		else if (line.find("normalMap: ") == 0)
		{
			m_normalMap = new Texture(line.substr(strlen("normalMap: ")));// ResourceManager::Get()->GetTexture(line.substr(strlen("normalMap: ")));
			if (m_normalMap)
				m_normalMap->setSlot(m_slot * 3 + 1);
		}

		//specularMap
		else if (line.find("specularMap: ") == 0)
		{
			m_specularMap = new Texture(line.substr(strlen("specularMap: ")));// ResourceManager::Get()->GetTexture(line.substr(strlen("specularMap: ")));
			if (m_specularMap)
				m_specularMap->setSlot(m_slot * 3 + 2);
		}
		//Color
		else if (line.find("color: ") == 0)
		{
			color = Vector4::Parse(line.substr(strlen("color: ")));
		}

		//Reflectivity
		else if (line.find("ref: ") == 0)
		{
			reflectivity = Math::Clamp01(numf(line.substr(strlen("ref: "))));
		}

		//Smoothness
		else if (line.find("smo: ") == 0)
		{
			smoothness = Math::ClampPos(numf(line.substr(strlen("smo: "))));
		}
	}
	file.close();
}

Material::Material(const std::string& textureName, Vector4 color, float reflectivity, float smoothness) : color(color), reflectivity(reflectivity), smoothness(smoothness)
{
	m_texture = ResourceManager::Get()->GetTexture(textureName);
}

Material::~Material()
{
}

void Material::Bind()
{
	if (m_texture)
		m_texture->Bind();
	if (m_normalMap)
		m_normalMap->Bind();
	if (m_specularMap)
		m_specularMap->Bind();
}

void Material::Unbind()
{
	m_texture->Unbind();
}

void Material::setSlot(unsigned int slot)
{
	m_slot = slot;
	m_texture->setSlot(m_slot * 3);
	m_normalMap->setSlot(m_slot * 3 + 1);
	m_specularMap->setSlot(m_slot * 3 + 2);
}

MaterialType Material::generateBuffer()
{
	unsigned int tex = 0;
	unsigned int norm = 0;
	unsigned int spec = 0;
	if (m_texture)
		tex = m_texture->getSlot();
	if (m_normalMap)
		norm = m_normalMap->getSlot();
	if (m_specularMap)
		spec = m_specularMap->getSlot();

	return MaterialType(tex, norm, spec);
}


