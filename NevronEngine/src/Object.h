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
	rsc<Shader> m_shader;
	rsc<Model> m_model;
	rsc<Material> m_material;
	rsc<Transform> m_transform;
	rsc<Rigidbody> m_rigidbody;
public:
	//If added component is shader, model, material, transform or rigibody it will replace the current one
	void AddComponent(rsc<Component> component);
	template <typename R>
	void RemoveComponent()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			if (typeid(*m_components[i]) == typeid(R))
			{
				m_components.erase(m_components.begin() + i);
				return;
			}
		}
	}

	template <typename R>
	void RemoveComponents()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			if (typeid(*m_components[i]) == typeid(R))
			{
				m_components.erase(m_components.begin() + i);
			}
		}
	}

	template<typename C> 
	rsc<C> GetComponent()
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			if (typeid(*m_components[i]) == typeid(C))
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
};


