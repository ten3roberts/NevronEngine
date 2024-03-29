#pragma once
#include <src/pch.h>

#include <Graphics/Texture.h>

//Matches the shader representation of Material
struct MaterialType
{
	MaterialType(unsigned int texture, unsigned int normalMap, unsigned int specularMap) : texture(texture), normalMap(normalMap), specularMap(specularMap) {}
	unsigned int texture, normalMap, specularMap;
};

class Material : public Component
{
	unsigned int m_slot;
	std::string m_filepath;

	rsc<Texture> m_texture;
	rsc<Texture> m_normalMap;
	rsc<Texture> m_specularMap;
public:
	Vector4 color;

	//Specular lightning
	float reflectivity;
	//Goes from 0 to infinity
	float smoothness;
public:
	Material(const std::string& name);
	Material(const std::string& textureName, Vector4 color, float reflectivity, float smoothness);
	~Material();

	rsc_weak<Texture> getTexture() const { return m_texture; }
	//Also assigns the correct slot
	void setTexture(rsc_weak<Texture> texture) { m_texture = texture; m_texture->setSlot(m_slot * 3); }

	rsc_weak<Texture> getNormalMap() { return m_normalMap; }
	//Also assigns the correct slot
	void setNormalMap(rsc_weak<Texture> normalMap) { m_normalMap = normalMap; m_normalMap->setSlot(m_slot * 3 + 1); }

	rsc_weak<Texture> getSpecularMap() { return m_specularMap; }
	//Also assigns the correct slot
	void setSpecularMap(rsc_weak<Texture> specularMap) { m_specularMap = specularMap; m_specularMap->setSlot(m_slot * 3 + 2); }


	void Bind();
	void Unbind();

	unsigned int getSlot() { return m_slot; }
	void setSlot(unsigned int slot);

	MaterialType generateBuffer();

	//Returns a texture by index; 0 : texture, 1 : normalMap, 2 : specularMap
	rsc<Texture> operator[](int index)
	{

		switch (index)
		{
		case 0:
			return m_texture;
		case 1:
			return m_normalMap;
		case 2:
			return m_specularMap;
		default:
			return nullptr;
		}
	}
private:
};



