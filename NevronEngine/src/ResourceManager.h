#pragma once

#include "Component.h"
#include <Graphics/Shader.h>
#include "Model.h"
#include <Graphics/Texture.h>

#include <vector>


class ResourceManager
{
	friend class Resource;
	friend class Component;
private:
	std::vector<Shader*> m_shaders;
	std::vector<Model*> m_models;
	std::vector<Texture*> m_textures;
public:
	void AddShader(Shader* shader);
	void AddModel(Model* model);
	void AddTexture(Texture* texture);

	//Will return the shader by name. If it doesn't exist it will attempt to load it from file
	Shader* GetShaderByName();

	//Will return the model by name. If it doesn't exist it will attempt to load it from file
	Model* GetModelByName();

	//Will return the texture by name. If it doesn't exist it will attempt to load it from file
	Texture* GetTextureByName();

	Shader* GetShaderByID();
	Model* GetModelByID();
	Texture* GetTextureByID();
};