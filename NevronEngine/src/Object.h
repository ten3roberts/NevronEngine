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
		rsc<A, false> tmp = ResourceManager::GetResource<A>(name);
		if (!tmp)
		{
			return false;
		}
		AddComponent(tmp);
		return true;
	}

	template <typename A>
	void AddComponent(rsc<A> component)
	{
		m_components.push_back(component);
	}

	template <>
	void AddComponent(rsc<Shader> shader)
	{
		m_components.push_back(shader);
		m_shader = shader;
	}

	template <>
	void AddComponent(rsc<Model> model)
	{
		m_components.push_back(model);
		m_model = model;
	}

	template <>
	void AddComponent(rsc<Material> material)
	{
		m_components.push_back(material);
		m_material = material;
	}

	template <>
	void AddComponent(rsc<Transform> transform)
	{
		m_components.push_back(transform);
		m_transform = transform;
	}

	template <>
	void AddComponent(rsc<Rigidbody> rigidbody)
	{
		m_components.push_back(rigidbody);
		m_rigidbody = rigidbody;
	}
#pragma endregion;

#pragma region RemoveComponent
	template <typename R>
	void RemoveComponent()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			R* cast = dynamic_cast<R*>(&m_components[i]);
			if (cast != nullptr) //If conversion was successful
			{
				m_components.erase(m_components.begin() + i);
				return;
			}
		}
	}

	template<>
	void RemoveComponent<Shader>()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			Shader* cast = dynamic_cast<Shader*>(&m_components[i]);
			if (cast != nullptr) //If conversion was successful
			{
				m_components.erase(m_components.begin() + i);
				return;
			}
		}
		m_shader = nullptr;
	}

	template<>
	void RemoveComponent<Model>()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			Model* cast = dynamic_cast<Model*>(&m_components[i]);
			if (cast != nullptr) //If conversion was successful
			{
				m_components.erase(m_components.begin() + i);
				return;
			}
		}
		m_model = nullptr;
	}

	template<>
	void RemoveComponent<Material>()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			Material* cast = dynamic_cast<Material*>(&m_components[i]);
			if (cast != nullptr) //If conversion was successful
			{
				m_components.erase(m_components.begin() + i);
				return;
			}
		}
		m_material = nullptr;
	}

	template<>
	void RemoveComponent<Transform>()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			Transform* cast = dynamic_cast<Transform*>(&m_components[i]);
			if (cast != nullptr) //If conversion was successful
			{
				m_components.erase(m_components.begin() + i);
				return;
			}
		}
		m_transform = nullptr;
	}

	template<>
	void RemoveComponent<Rigidbody>()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			Rigidbody* cast = dynamic_cast<Rigidbody*>(&m_components[i]);
			if (cast != nullptr) //If conversion was successful
			{
				m_components.erase(m_components.begin() + i);
				return;
			}
		}
		m_rigidbody = nullptr;
	}
#pragma endregion
	template <typename R>
	void RemoveComponents()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			R* cast = dynamic_cast<R*>(&m_components[i]);
			if (cast != nullptr) //If conversion was successful
			{
				m_components.erase(m_components.begin() + i);
			}
		}
	}

	template <>
	void RemoveComponents<Shader>()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			Shader* cast = dynamic_cast<Shader*>(&m_components[i]);
			if (cast != nullptr) //If conversion was successful
			{
				m_components.erase(m_components.begin() + i);
			}
		}
		m_shader = nullptr;
	}

	template <>
	void RemoveComponents<Model>()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			Model* cast = dynamic_cast<Model*>(&m_components[i]);
			if (cast != nullptr) //If conversion was successful
			{
				m_components.erase(m_components.begin() + i);
			}
		}
		m_model = nullptr;
	}

	template <>
	void RemoveComponents<Material>()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			Material* cast = dynamic_cast<Material*>(&m_components[i]);
			if (cast != nullptr) //If conversion was successful
			{
				m_components.erase(m_components.begin() + i);
			}
		}
		m_material = nullptr;
	}

	template <>
	void RemoveComponents<Transform>()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			Transform* cast = dynamic_cast<Transform*>(&m_components[i]);
			if (cast != nullptr) //If conversion was successful
			{
				m_components.erase(m_components.begin() + i);
			}
		}
		m_transform = nullptr;
	}

	template <>
	void RemoveComponents<Rigidbody>()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			Rigidbody* cast = dynamic_cast<Rigidbody*>(&m_components[i]);
			if (cast != nullptr) //If conversion was successful
			{
				m_components.erase(m_components.begin() + i);
			}
		}
		m_rigidbody = nullptr;
	}

#pragma region GetComponent
	rsc<Component> GetComponent(const std::string& name)
	{
		for (int i = 0; i < m_components.size(); i++)
			if (m_components[i]->getName() == name)
				return m_components[i];
	}
	template<typename C>
	rsc<C> GetComponent()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			C* cast = dynamic_cast<C*>(&m_components[i]);
			if (cast != nullptr) //If conversion was successful
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
	template<typename S>
	std::vector<rsc<S>> GetComponents()
	{
		std::vector<rsc<S>> result;
		for (int i = 0; i < m_components.size(); i++)
		{
			S* cast = dynamic_cast<S*>(&m_components[i]);
			if (cast != nullptr) //If conversion was successful
			{
				result.push_back(m_components[i]);
			}
		}
		return result;
	}

	//TODO template specialise script list
};


