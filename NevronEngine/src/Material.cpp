#include "Material.h"

#include <src/Utility.h>
#include <Math/Math.h>
#include <fstream>

using namespace Utility;

Material::Material(const std::string& name) : texture(nullptr), texture1(nullptr), texture2(nullptr), texture3(nullptr), color(Vector3::white), reflectivity(0), smoothness(0)
{
	m_name = strLead(getFilename(name, true), ".mat");

	std::ifstream file(FindFile(m_name));

	if (!file.is_open())
		return;

	std::string line;
	while (file.good())
	{
		std::getline(file, line);

		//Texture
		if (line.find("tex: ") == 0)
		{
			texture = new Texture(line.substr(5), 0);
		}

		//Texture1
		else if (line.find("tex1: ") == 0)
		{
			texture1 = new Texture(line.substr(6), 1);
		}

		//Texture2
		else if (line.find("tex2: ") == 0)
		{
			texture2 = new Texture(line.substr(6), 2);
		}

		//Texture3
		else if (line.find("tex3: ") == 0)
		{
			texture3 = new Texture(line.substr(6), 3);
		}

		//Color
		else if (line.find("col: ") == 0)
		{
			color = Vector4::Parse(line.substr(6));
		}

		//Reflectivity
		else if (line.find("ref: ") == 0)
		{
			reflectivity = Math::Clamp01(num(line.substr(6)));
		}

		//Smoothness
		else if (line.find("smo: ") == 0)
		{
			smoothness = Math::ClampPos(num(line.substr(6)));
		}
	}
	file.close();
}

Material::Material(const std::string& textureName, Vector4 color, float reflectivity, float smoothness) : color(color), reflectivity(reflectivity), smoothness(smoothness)
{
	texture = new Texture(textureName);
}

Material::~Material()
{
	if (texture)
		delete texture;

	if (texture1)
		delete texture1;

	if (texture2)
		delete texture2;

	if (texture3)
		delete texture3;
}

void Material::Bind() const
{
	if (texture)
		texture->Bind();
	if (texture1)
		texture1->Bind();
	if (texture2)
		texture2->Bind();
	if (texture3)
		texture3->Bind();
}

void Material::Unbind() const
{
	texture->Unbind();
}
