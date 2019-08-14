#pragma once

#include "Component.h"
#include <src\Logger.h>
#include <vector>

class Model;
class Shader;
class Material;
class UniformBuffer;

#define RSCMAN ResourceManager::Get();

class ResourceManager
{
private:

	//Object resources; A.K.A components
	std::vector<rsc<Shader, false>> m_shaders;

	std::vector<rsc<Model, false>> m_models;

	std::vector<rsc<Material, false>> m_materials;

	//Graphical resources

	std::vector<rsc<UniformBuffer, false>> m_uniformBuffers;

	//Keeps track of the availabale uniform buffer slots. The last element in the array indicates that all above those are free
	std::vector<unsigned int> m_UBOSlots;

	ResourceManager();
	~ResourceManager();

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

	//Will return a uniformBuffer by name. If it doesn't exist it will attempt to load it from file
	rsc<UniformBuffer> GetUBO(const std::string& name);
	//Will return a uniformBuffer by ID. Returns nullptr if it doesn't exist
	rsc<UniformBuffer> GetUBO(GUID ID);

	bool DeleteShader(const std::string& name);
	bool DeleteShader(GUID ID);

	bool DeleteModel(const std::string& name);
	bool DeleteModel(GUID ID);

	bool DeleteMaterial(const std::string& name);
	bool DeleteMaterial(GUID ID);

	bool DeleteUBO(const std::string& name);
	bool DeleteUBO(GUID ID);
public:
#pragma region Get resource by name
	//Will return a shader by name. If it doesn't exist it will attempt to load it from file
	template <typename R>
	inline rsc<R> GetResource(const std::string& name) = delete;
	template<>
	inline rsc<Shader> GetResource<Shader>(const std::string& name) { return GetShader(name); }
	
	template<>
	//Will return a model by name. If it doesn't exist it will attempt to load it from file
	inline rsc<Model> GetResource<Model>(const std::string& name) { return GetModel(name); }

	template<>
	//Will return a material by name. If it doesn't exist it will attempt to load it from file
	inline rsc<Material> GetResource<Material>(const std::string& name) { return GetMaterial(name); }

	template <>
	//Will return a uniformBuffer by name. If it doesn't exist it will attempt to load it from file
	inline rsc<UniformBuffer> GetResource<UniformBuffer>(const std::string& name) { return GetUBO(name); }
#pragma endregion
#pragma region Get resource by ID

	template <typename R>
	inline rsc<R> GetResource(GUID ID) = delete;
	template<>
	inline rsc<Shader> GetResource<Shader>(GUID ID) { return GetShader(ID); }
	

	template<>
	//Will return a model by ID. Returns nullptr if it doesn't exist
	inline rsc<Model> GetResource<Model>(GUID ID) { return GetModel(ID); }


	template<>
	//Will return a material by ID. Returns nullptr if it doesn't exist
	inline rsc<Material> GetResource<Material>(GUID ID) { return GetMaterial(ID); }
	

	template <>
	//Will return a uniformBuffer by ID. Returns nullptr if it doesn't exist
	inline rsc<UniformBuffer> GetResource<UniformBuffer>(GUID ID) { return GetUBO(ID); }

#pragma endregion
	//Will not free the memory but remove it from the list. rsc wrapper deletes the internal pointer. Returns true if resource could be deleted
#pragma region Freeing of resources by name

	template <typename D>
	inline bool DeleteResource(const std::string& name) {};

	template <>
	inline bool DeleteResource<Shader>(const std::string& name) { return DeleteShader(name); }
	
	template <>
	inline bool DeleteResource<Model>(const std::string& name) { return DeleteModel(name); }
	
	template <>
	inline bool DeleteResource<Material>(const std::string& name) { return DeleteMaterial(name); }
	

	template <>
	inline bool DeleteResource<UniformBuffer>(const std::string& name) { return DeleteUBO(name); }
#pragma endregion

#pragma region Freeing of resources by ID

	template <typename D>
	inline bool DeleteResource(GUID ID) = delete;

	template <>
	inline bool DeleteResource<Shader>(GUID ID) { return DeleteShader(ID); }
	

	template <>
	inline bool DeleteResource<Model>(GUID ID) { return DeleteModel(ID); }


	template <>
	inline bool DeleteResource<Material>(GUID ID) { return DeleteMaterial(ID); }
	

	template <>
	inline bool DeleteResource<UniformBuffer>(GUID ID) { return DeleteUBO(ID); }
#pragma endregion
};


