#pragma once

#include <Graphics/Texture.h>
#include <Math\Vector4.h>

class Material : public Component
{
public:
	Material(const std::string& name);
	Material(const std::string& textureName, Vector4 color, float reflectivity, float smoothness);
	~Material();

	Texture* texture;
	Texture* texture1;
	Texture* texture2;
	Texture* texture3;
	Vector4 color;

	//Specular lightning
	float reflectivity;
	float reflectivity1;
	float reflectivity2;
	//Goes from 0 to infinity
	float smoothness;
	float smoothness1;
	float smoothness2;

	void Bind() const;
	void Unbind() const;
};

