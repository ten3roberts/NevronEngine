#pragma once
#include <src/Component.h>
#include <src/ResourceManager.h>
#include <Graphics/Shader.h>
#include <Graphics/Model.h>
#include <Graphics/Material.h>
#include <src/Transform.h>
#include <src/Rigidbody.h>
#include <vector>

//Object is a base class of 
class Object
{
private:
	void RemoveSpecialized(rsc<Component> component);
	std::vector<rsc<Component>> m_components;

	//Fast access
	rsc<Shader, false> m_shader;
	rsc<Model, false> m_model;
	rsc<Material, false> m_material;
	rsc<Transform, false> m_transform;
	rsc<Rigidbody, false> m_rigidbody;
public:
#pragma region AddComponent
	//If added component is shader, model, material, transform or rigibody it will replace the current one
	void AddComponent(rsc<Component> component);

	//Resource manager integration; return true if resource was successfully added
	template <typename A>
	bool AddComponent(const std::string& name)
	{
		rsc<A, false> tmp = ResourceManager::Get()->GetResource<A>(name);
		if (!tmp)
		{
			return false;
		}
		AddComponent(tmp);
		return true;
	}
#pragma endregion;

#pragma region RemoveComponent
	//Removes the first component of R or derived from R type
	template <typename R>
	void RemoveComponent()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			R* cast = dynamic_cast<R*>(&m_components[i]);
			//If conversion was successful and name was correct
			if (cast != nullptr)
			{
				RemoveSpecialized(m_components[i]);
				m_components.erase(m_components.begin() + i);
				return;
			}
		}
	}

	//Removes the first component of R or derived from R type with the correct name
	template <typename R = Component>
	void RemoveComponent(const std::string & name)
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			R* cast = dynamic_cast<R*>(&m_components[i]);
			//If conversion was successful
			if (cast != nullptr && m_components[i]->getName() == name)
			{
				RemoveSpecialized(m_components[i]);
				m_components.erase(m_components.begin() + i);
				return;
			}
		}
	}	
#pragma endregion
#pragma region RemoveComponents
	template <typename R>
	//Removes all components of R or derived from R type
	void RemoveComponents()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			R* cast = dynamic_cast<R*>(&m_components[i]);
			//If conversion was successful
			if (cast != nullptr)
			{
				RemoveSpecialized(m_components[i]);
				m_components.erase(m_components.begin() + i);
			}
		}
	}

	template <typename R = Component>
	//Removes all components of R or derived from R type
	void RemoveComponents(const std::string& name)
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			R* cast = dynamic_cast<R*>(&m_components[i]);
			//If conversion was successful and name is correct
			if (cast != nullptr && m_components[i]->getName() == name)
			{
				RemoveSpecialized(m_components[i]);
				m_components.erase(m_components.begin() + i);
			}
		}
	}
#pragma endregion
#pragma region GetComponent
	template<typename C>
	rsc<C> GetComponent(const std::string& name)
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			C* cast = dynamic_cast<C*>(&m_components[i]);
			//If conversion was successful and name is correct
			if (cast != nullptr && m_components[i]->getName() == name)
				return m_components[i];
		}
		return rsc<C>(nullptr);
	}

	template<typename C>
	rsc<C> GetComponent()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			C* cast = dynamic_cast<C*>(&m_components[i]);
			//If conversion was successful
			if (cast != nullptr)
				return m_components[i];
		}
		return rsc<C>(nullptr);
	}

	template<>
	rsc<Shader> GetComponent()
	{
		return m_shader;
	}

	template<>
	rsc<Model> GetComponent()
	{
		return m_model;
	}

	template<>
	rsc<Material> GetComponent()
	{
		return m_material;
	}

	template<>
	rsc<Transform> GetComponent()
	{
		return m_transform;
	}

	template<>
	rsc<Rigidbody> GetComponent()
	{
		return m_rigidbody;
	}
#pragma endregion
#pragma region GetComponents
	template<typename S = Component>
	std::vector<rsc<S>> GetComponents()
	{
		std::vector<rsc<S>> result;
		for (int i = 0; i < m_components.size(); i++)
		{
			S* cast = dynamic_cast<S*>(&m_components[i]);
			//If conversion was successful
			if (cast != nullptr)
			{
				result.push_back(m_components[i]);
			}
		}
		return result;
	}

	template<typename S = Component>
	std::vector<rsc<S>> GetComponents(const std::string& name)
	{
		std::vector<rsc<S>> result;
		for (int i = 0; i < m_components.size(); i++)
		{
			S* cast = dynamic_cast<S*>(&m_components[i]);
			//If conversion was successful and name is correct
			if (cast != nullptr && m_components[i]->getName() == name) 
			{
				result.push_back(m_components[i]);
			}
		}
		return result;
	}

	//TODO template specialise script list
#pragma endregion
};


