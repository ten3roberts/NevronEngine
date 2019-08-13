#include "Object.h"
#include <src/ResourceManager.h>

void Object::AddComponent(rsc<Component> component)
{
	if (dynamic_cast<Shader*>(&component))
	{
		if (m_shader)
			RemoveComponents<Shader>();
		m_shader = component;
	}
	else if (dynamic_cast<Model*>(&component))
	{
		if (m_model)
			RemoveComponents<Model>();
		m_model = component;
	}
	else if (dynamic_cast<Material*>(&component))
	{
		if (m_material)
			RemoveComponents<Material>();
		m_material = component;
	}
	else if (dynamic_cast<Transform*>(&component))
	{
		if (m_transform)
			RemoveComponents<Transform>();
		m_transform = component;
	}
	else if (dynamic_cast<Rigidbody*>(&component))
	{
		if (m_rigidbody)
			RemoveComponents<Rigidbody>();
		m_rigidbody = component;
	}
	m_components.push_back(component);
}

void Object::RemoveSpecialized(rsc<Component> component)
{
	if (dynamic_cast<Shader*>(&component))
		m_shader = nullptr;
	else if (dynamic_cast<Model*>(&component))
		m_model = nullptr;
	else if (dynamic_cast<Material*>(&component))
		m_material = nullptr;
	else if (dynamic_cast<Transform*>(&component))
		m_transform = nullptr;
	else if (dynamic_cast<Rigidbody*>(&component))
		m_rigidbody = nullptr;
}
