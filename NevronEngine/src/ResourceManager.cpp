#include "ResourceManager.h"
#include <src/Logger.h>

#include <Graphics/Shader.h>
#include <Graphics/Model.h>
#include <Graphics/Material.h>
#include <Graphics/Texture.h>


#include <Graphics/UniformBuffer.h>

ResourceManager::ResourceManager()
{
	m_UBOSlots.push_back(0);
}

ResourceManager* ResourceManager::Get()
{
	static ResourceManager* instance;
	if (!instance)
		instance = new ResourceManager();
	return instance;
}

void ResourceManager::Refresh()
{
	for (int i = 0; i < m_shaders.size(); i++)
	{
		if (m_shaders[i].getRefCount() == 0)
			DeleteResource<Shader>(m_shaders[i]->getGUID());
	}

	for (int i = 0; i < m_models.size(); i++)
	{
		if (m_models[i].getRefCount() == 0)
			DeleteResource<Model>(m_models[i]->getGUID());
	}

	for (int i = 0; i < m_materials.size(); i++)
	{
		if (m_materials[i].getRefCount() == 0)
			DeleteResource<Material>(m_materials[i]->getGUID());
	}

	for (int i = 0; i < m_textures.size(); i++)
	{
		if (m_textures[i].getRefCount() == 0)
			DeleteResource<Texture>(m_textures[i]->getGUID());
	}

	for (int i = 0; i < m_uniformBuffers.size(); i++)
	{
		if (m_uniformBuffers[i].getRefCount() == 0)
			DeleteResource<UniformBuffer>(m_uniformBuffers[i]->getGUID());
	}
}

unsigned int ResourceManager::GetUBOSlot()
{
	return m_UBOSlots[0];

	//If there were more than one available remove the first one which was used
	if (m_UBOSlots.size() > 1)
		m_UBOSlots.erase(m_UBOSlots.begin());
	//If there was only one available slot raise it up one. This one indicates the lower bound. All above are available
	else
	{
		m_UBOSlots[0] += 1;
	}
}

void ResourceManager::FreeUBOSlot(unsigned int slot)
{
	for (int i = 0; i < m_UBOSlots.size(); i++)
	{
		if (m_UBOSlots[i] == slot)
		{
			LogS("ResourceManager", "Slot %d has already been freed", slot);
			return;
		}
		m_UBOSlots.insert(m_UBOSlots.begin(), slot);
	}
}

rsc<Shader> ResourceManager::GetShader(const std::string& name)
{
	for (unsigned int i = 0; i < m_shaders.size(); i++)
	{
		if (m_shaders[i]->getName() == name)
			return m_shaders[i];
	}
	LogS("ResourceManager", "No shader exists with name %s, creating", name);
	Shader* shader = new Shader(name);
	if (!shader->getValid())
	{
		delete shader;
		return nullptr;
	}
	return m_shaders.emplace_back(shader);
}

rsc<Shader> ResourceManager::GetShader(GUID ID)
{
	for (unsigned int i = 0; i < m_shaders.size(); i++)
	{
		if (m_shaders[i]->getGUID() == ID)
			return m_shaders[i];
	}
	return nullptr;
}

rsc<Model> ResourceManager::GetModel(const std::string& name)
{
	for (unsigned int i = 0; i < m_models.size(); i++)
	{
		if (m_models[i]->getName() == name)
			return m_models[i];
	}
	LogS("ResourceManager", "No model exists with name %s, creating", name);
	Model* model = new Model(name);
	if (!model->getValid())
	{
		delete model;
		return nullptr;
	}
	return m_models.emplace_back(model);
}

rsc<Model> ResourceManager::GetModel(GUID ID)
{
	for (unsigned int i = 0; i < m_models.size(); i++)
	{
		if (m_models[i]->getGUID() == ID)
			return m_models[i];
	}
	return nullptr;
}

rsc<Material> ResourceManager::GetMaterial(const std::string& name)
{
	for (unsigned int i = 0; i < m_materials.size(); i++)
	{
		if (m_materials[i]->getName() == name)
			return m_materials[i];
	}
	LogS("ResourceManager", "No material exists with name %s, creating", name);
	Material* material = new Material(name);
	if (!material->getValid())
	{
		delete material;
		return nullptr;
	}
	return m_materials.emplace_back(material);
}

rsc<Material> ResourceManager::GetMaterial(GUID ID)
{
	for (unsigned int i = 0; i < m_materials.size(); i++)
	{
		if (m_materials[i]->getGUID() == ID)
			return m_materials[i];
	}
	return nullptr;
}

rsc<Texture> ResourceManager::GetTexture(const std::string& name)
{
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		if (m_textures[i]->getName() == name)
			return m_textures[i];
	}
	LogS("ResourceManager", "No texture exists with name %s, creating", name);
	Texture* texture = new Texture(name);
	if (!texture)
	{
		delete texture;
		return nullptr;
	}
	return m_textures.emplace_back(texture);
}

rsc<Texture> ResourceManager::GetTexture(GUID ID)
{
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		if (m_textures[i]->getGUID() == ID)
			return m_textures[i];
	}
	return nullptr;
}

rsc<UniformBuffer> ResourceManager::GetUBO(const std::string& name)
{
	for (unsigned int i = 0; i < m_uniformBuffers.size(); i++)
	{
		if (m_uniformBuffers[i]->getName() == name)
			return m_uniformBuffers[i];
	}
	LogS("ResourceManager", "No uniform buffer exists with name %s, creating", name);
	UniformBuffer* uBuffer = new UniformBuffer(name);
	return m_uniformBuffers.emplace_back(rsc<UniformBuffer>(uBuffer));
}

rsc<UniformBuffer> ResourceManager::GetUBO(GUID ID)
{
	for (unsigned int i = 0; i < m_uniformBuffers.size(); i++)
	{
		if (m_uniformBuffers[i]->getGUID() == ID)
			return m_uniformBuffers[i];
	}
	return nullptr;
}



bool ResourceManager::DeleteShader(const std::string& name)
{
	for (int i = 0; i < m_shaders.size(); i++)
	{
		if (m_shaders[i]->getName() == name)
		{
			LogS("ResourceManager", "Deleting shader %s", m_shaders[i]->getName());
			m_shaders.erase(m_shaders.begin() + i);
			return true;
		}
	}
	return false;
}

bool ResourceManager::DeleteShader(GUID ID)
{
	for (int i = 0; i < m_shaders.size(); i++)
	{
		if (m_shaders[i]->getGUID() == ID)
		{
			LogS("ResourceManager", "Deleting shader %s", m_shaders[i]->getName());
			m_shaders.erase(m_shaders.begin() + i);
			return true;
		}
	}
	return false;
}

bool ResourceManager::DeleteModel(const std::string& name)
{
	for (int i = 0; i < m_models.size(); i++)
	{
		if (m_models[i]->getName() == name)
		{
			LogS("ResourceManager", "Deleting model %s", m_models[i]->getName());
			m_models.erase(m_models.begin() + i);
			return true;
		}
	}
	return false;
}

bool ResourceManager::DeleteModel(GUID ID)
{
	for (int i = 0; i < m_models.size(); i++)
	{
		if (m_models[i]->getGUID() == ID)
		{
			LogS("ResourceManager", "Deleting model %s", m_models[i]->getName());
			m_models.erase(m_models.begin() + i);
			return true;
		}
	}
	return false;
}
bool ResourceManager::DeleteMaterial(const std::string& name)
{
	for (int i = 0; i < m_materials.size(); i++)
	{
		if (m_materials[i]->getName() == name)
		{
			LogS("ResourceManager", "Deleting material %s", m_materials[i]->getName());
			m_materials.erase(m_materials.begin() + i);
			return true;
		}
	}
	return false;
}

bool ResourceManager::DeleteMaterial(GUID ID)
{
	for (int i = 0; i < m_materials.size(); i++)
	{
		if (m_materials[i]->getGUID() == ID)
		{
			LogS("ResourceManager", "Deleting material %s", m_materials[i]->getName());
			m_materials.erase(m_materials.begin() + i);
			return true;
		}
	}
	return false;
}

bool ResourceManager::DeleteTexture(const std::string& name)
{
	for (int i = 0; i < m_textures.size(); i++)
	{
		if (m_textures[i]->getName() == name)
		{
			LogS("ResourceManager", "Deleting texture %s", m_textures[i]->getName());
			m_textures.erase(m_textures.begin() + i);
			return true;
		}
	}
	return false;
}

bool ResourceManager::DeleteTexture(GUID ID)
{
	for (int i = 0; i < m_textures.size(); i++)
	{
		if (m_textures[i]->getGUID() == ID)
		{
			LogS("ResourceManager", "Deleting texture %s", m_textures[i]->getName());
			m_textures.erase(m_textures.begin() + i);
			return true;
		}
	}
	return false;
}

bool ResourceManager::DeleteUBO(const std::string& name)
{
	for (int i = 0; i < m_uniformBuffers.size(); i++)
	{
		if (m_uniformBuffers[i]->getName() == name)
		{
			LogS("ResourceManager", "Deleting UBO %s", m_uniformBuffers[i]->getName());
			m_uniformBuffers.erase(m_uniformBuffers.begin() + i);
			return true;
		}
	}
	return false;
}

bool ResourceManager::DeleteUBO(GUID ID)
{
	for (int i = 0; i < m_uniformBuffers.size(); i++)
	{
		if (m_uniformBuffers[i]->getGUID() == ID)
		{
			LogS("ResourceManager", "Deleting UBO %s", m_uniformBuffers[i]->getName());
			m_uniformBuffers.erase(m_uniformBuffers.begin() + i);
			return true;
		}
	}
	return false;
}


