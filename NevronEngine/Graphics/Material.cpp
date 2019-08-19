#include "Material.h"

#include <src/ResourceManager.h>
#include <src/Utility.h>
#include <Math/Math.h>
#include <fstream>

using namespace Utility;

Material::Material(const std::string& name) : m_slot(0), m_texture(nullptr), m_normalMap(nullptr), m_specularMap(nullptr)
{
	m_name = getFilename(name, false);

	std::ifstream file(FindFile(strLead(m_name, ".mat")));

	if (!file.is_open())
		return;

	std::string line;
	while (file.good())
	{
		std::getline(file, line);

		//Texture
		if (line.find("texture: ") == 0)
		{
			m_texture = ResourceManager::Get()->GetTexture(line.substr(strlen("texture: ")));
			if (m_texture)
				m_texture->setSlot(m_slot * 3);
		}

		//normalMap
		else if (line.find("normalMap: ") == 0)
		{
			m_normalMap = ResourceManager::Get()->GetTexture(line.substr(strlen("normalMap: ")));
			if (m_normalMap)
				m_normalMap->setSlot(m_slot * 3 + 1);
		}

		//specularMap
		else if (line.find("specularMap: ") == 0)
		{
			m_specularMap = ResourceManager::Get()->GetTexture(line.substr(strlen("specularMap: ")));
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
			reflectivity = Math::Clamp01(num(line.substr(strlen("ref: "))));
		}

		//Smoothness
		else if (line.find("smo: ") == 0)
		{
			smoothness = Math::ClampPos(num(line.substr(strlen("smo: "))));
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

void Material::Bind() const
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


