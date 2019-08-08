#include "ResourceManager.h"
#include <src/Logger.h>

#include <Graphics/Shader.h>
#include <Graphics/Model.h>
#include <Graphics/Material.h>
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
		if (m_shaders[i].getReferenceCount() == 0)
			DeleteResource<Shader>(m_shaders[i]->getID());
	}

	for (int i = 0; i < m_models.size(); i++)
	{
		if (m_models[i].getReferenceCount() == 0)
			DeleteResource<Model>(m_models[i]->getID());
	}

	for (int i = 0; i < m_materials.size(); i++)
	{
		if (m_materials[i].getReferenceCount() == 0)
			DeleteResource<Material>(m_materials[i]->getID());
	}

	for (int i = 0; i < m_uniformBuffers.size(); i++)
	{
		if (m_uniformBuffers[i].getReferenceCount() == 0)
			DeleteResource<Model>(m_uniformBuffers[i]->getID());
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
			Logf("ResourceManager", "Slot %d has already been freed", slot);
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
	Logf("ResourceManager", "No shader exists with name %s, creating", name.c_str());
	Shader* shader = new Shader(name);
	return m_shaders.emplace_back(shader, false);
}

rsc<Shader> ResourceManager::GetShader(unsigned int ID)
{
	for (unsigned int i = 0; i < m_shaders.size(); i++)
	{
		if (m_shaders[i]->getID() == ID)
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
	Logf("ResourceManager", "No model exists with name %s, creating", name.c_str());
	Model* model = new Model(name);
	return m_models.emplace_back(model, false);
}

rsc<Model> ResourceManager::GetModel(unsigned int ID)
{
	for (unsigned int i = 0; i < m_models.size(); i++)
	{
		if (m_models[i]->getID() == ID)
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
	Logf("ResourceManager", "No material exists with name %s, creating", name.c_str());
	Material* material = new Material(name);
	return m_materials.emplace_back(material, false);
}

rsc<Material> ResourceManager::GetMaterial(unsigned int ID)
{
	for (unsigned int i = 0; i < m_materials.size(); i++)
	{
		if (m_materials[i]->getID() == ID)
			return m_materials[i];
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
	Logf("ResourceManager", "No material exists with name %s, creating", name.c_str());
	UniformBuffer* uBuffer = new UniformBuffer(name);
	return m_uniformBuffers.emplace_back(rsc<UniformBuffer>(uBuffer, false));
}

rsc<UniformBuffer> ResourceManager::GetUBO(unsigned int ID)
{
	for (unsigned int i = 0; i < m_uniformBuffers.size(); i++)
	{
		if (m_uniformBuffers[i]->getID() == ID)
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
			Logf("ResourceManager", "Deleting shader %s", m_shaders[i]->getName().c_str());
			m_shaders.erase(m_shaders.begin() + i);
			return true;
		}
	}
	return false;
}

bool ResourceManager::DeleteShader(unsigned int ID)
{
	for (int i = 0; i < m_shaders.size(); i++)
	{
		if (m_shaders[i]->getID() == ID)
		{
			Logf("ResourceManager", "Deleting shader %s", m_shaders[i]->getName().c_str());
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
			Logf("ResourceManager", "Deleting model %s", m_models[i]->getName().c_str());
			m_models.erase(m_models.begin() + i);
			return true;
		}
	}
	return false;
}

bool ResourceManager::DeleteModel(unsigned int ID)
{
	for (int i = 0; i < m_models.size(); i++)
	{
		if (m_models[i]->getID() == ID)
		{
			Logf("ResourceManager", "Deleting model %s", m_models[i]->getName().c_str());
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
			Logf("ResourceManager", "Deleting material %s", m_materials[i]->getName().c_str());
			m_materials.erase(m_materials.begin() + i);
			return true;
		}
	}
	return false;
}

bool ResourceManager::DeleteMaterial(unsigned int ID)
{
	for (int i = 0; i < m_materials.size(); i++)
	{
		if (m_materials[i]->getID() == ID)
		{
			Logf("ResourceManager", "Deleting material %s", m_materials[i]->getName().c_str());
			m_materials.erase(m_materials.begin() + i);
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
			Logf("ResourceManager", "Deleting UBO %s", m_uniformBuffers[i]->getName().c_str());
			m_uniformBuffers.erase(m_uniformBuffers.begin() + i);
			return true;
		}
	}
	return false;
}

bool ResourceManager::DeleteUBO(unsigned int ID)
{
	for (int i = 0; i < m_uniformBuffers.size(); i++)
	{
		if (m_uniformBuffers[i]->getID() == ID)
		{
			Logf("ResourceManager", "Deleting UBO %s", m_uniformBuffers[i]->getName().c_str());
			m_uniformBuffers.erase(m_uniformBuffers.begin() + i);
			return true;
		}
	}
	return false;
}