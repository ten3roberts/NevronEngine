#pragma once
#include <src/pch.h>

#include "Component.h"

#include <Graphics/Model.h>
#include <Graphics/Shader.h>
#include <Graphics/Material.h>
#include <Graphics/UniformBuffer.h>

class ResourceManager
{
private:

	//Entity resources; A.K.A components
	std::vector<rsc_weak<Shader>> m_shaders;

	std::vector<rsc_weak<Model>> m_models;

	std::vector<rsc_weak<Material>> m_materials;

	std::vector<rsc_weak<Texture>> m_textures;

	//Graphical resources

	std::vector<rsc_weak<UniformBuffer>> m_uniformBuffers;

	//Keeps track of the availabale uniform buffer slots. The last element in the array indicates that all above those are free
	std::vector<unsigned int> m_UBOSlots;

	ResourceManager();

public:
	static ResourceManager* Get();

	//Will search through the resources and delete those that aren't used by anything
	void Refresh();

	unsigned int GetUBOSlot();
	void FreeUBOSlot(unsigned int slot);
public:
	//Will return a shader by name. If it doesn't exist it will attempt to load it from file
	rsc<Shader> GetShader(const std::string& name);
	//Will return a shader by ID. Returns nullptr if it doesn't exist
	rsc<Shader> GetShader(GUID ID);

	//Will return a model by name. If it doesn't exist it will attempt to load it from file
	rsc<Model> GetModel(const std::string& name);
	//Will return a model by ID. Returns nullptr if it doesn't exist
	rsc<Model> GetModel(GUID ID);

	//Will return a material by name. If it doesn't exist it will attempt to load it from file
	rsc<Material> GetMaterial(const std::string& name);
	//Will return a material by ID. Returns nullptr if it doesn't exist
	rsc<Material> GetMaterial(GUID ID);

	//Will return a texture by name. If it doesn't exist it will attempt to load it from file
	rsc<Texture> GetTexture(const std::string& name);
	//Will return a texture by ID. Returns nullptr if it doesn't exist
	rsc<Texture> GetTexture(GUID ID);

	rsc<UniformBuffer> CreateUBO(const std::string& name, const void* data, size_t size);
	inline rsc<UniformBuffer> CreateUBO(const std::string& name, size_t size) { return CreateUBO(name, nullptr, size); }

	//Will return a uniformBuffer by name. Returns nullptr if it doesn't exist since size needs to be known
	rsc<UniformBuffer> GetUBO(const std::string& name);

	//Will return a uniformBuffer by ID. Returns nullptr if it doesn't exist
	rsc<UniformBuffer> GetUBO(GUID ID);


	bool DeleteShader(const std::string& name);
	bool DeleteShader(GUID ID);

	bool DeleteModel(const std::string& name);
	bool DeleteModel(GUID ID);

	bool DeleteMaterial(const std::string& name);
	bool DeleteMaterial(GUID ID);

	bool DeleteTexture(const std::string& name);
	bool DeleteTexture(GUID ID);

	bool DeleteUBO(const std::string& name);
	bool DeleteUBO(GUID ID);
};


