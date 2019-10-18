#pragma once
#include <src/pch.h>

#include <src/Component.h>
#include <src/ResourceManager.h>
#include <Graphics/Shader.h>
#include <Graphics/Model.h>
#include <Graphics/Material.h>
#include <src/Transform.h>
#include <src/Camera.h>
#include <src/Rigidbody.h>
class Script;

class Entity
{
private:
	std::string m_name;
	GUID m_GUID;

	//A string formatting the name and GUID
	std::string m_signature;
	std::vector<rsc<Component>> m_components;
	std::vector<rsc<Script>> m_scripts;
public:
	//Fast access
	rsc<Shader>			shader;
	rsc<Model>			model;
	rsc<Material>		material;
	rsc<Transform>		transform;
	rsc<Rigidbody>		rigidbody;
private:
	void RemoveSpecialized(rsc<Component> component);
	void RefreshComponents();
	void Init(const std::string& shader, const std::string& model, const std::string& material, Vector3 position, Quaternion rotation, Vector3 scale, std::vector<rsc<Component>> components);
public:
	Entity();
	Entity(const std::string& shader, const std::string model, const std::string material, std::vector<rsc<Component>> components);
	Entity(const std::string& shader, const std::string model, const std::string material, Vector3 position, Quaternion rotation, Vector3 scale, std::vector<rsc<Component>> components);

	void Update();
	void FixedUpdate();
	void EarlyUpdate();
	void Render(rsc_weak<Camera> camera);

	std::string getName() { return m_name; }
	void setName(const std::string& name) { m_name = name; m_signature = "Entity : " + m_name + ";" + m_GUID.getString(); }
	std::string getSignature() { return m_signature; }

	//If added component is shader, model, material, transform or rigibody it will replace the current one
	void AddComponent(rsc<Component> component);

	/*//Resource manager integration; return true if resource was successfully added
	template <typename A>
	bool AddComponent(const std::string& name)
	{
		rsc<A> tmp = ResourceManager::Get()->GetResource<A>(name);
		if (!tmp || !tmp->isValid())
		{
			LogE(m_signature, "Trying to add invalid %c; nothing changed", typeid(A).name());
			return false;
		}
		AddComponent(tmp);
		return true;
	}*/
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
	void RemoveComponents(const std::string & name)
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
	std::vector<rsc<S>> GetComponents(const std::string & name)
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


	template<>
	inline rsc<Shader> Entity::GetComponent()
	{
		return shader;
	}

	template<>
	inline rsc<Model> Entity::GetComponent()
	{
		return model;
	}

	template<>
	inline rsc<Material> Entity::GetComponent()
	{
		return material;
	}

	template<>
	inline rsc<Transform> Entity::GetComponent()
	{
		return transform;
	}

	template<>
	inline rsc<Rigidbody> Entity::GetComponent()
	{
		return rigidbody;
	}