#include "Object.h"

void Object::AddComponent(rsc<Component> component)
{
	if (typeid(*component) == typeid(Shader))
	{
		if (m_shader.Valid())
			RemoveComponents<Shader>();
		m_shader = component;
	}
	if (typeid(*component) == typeid(Model))
	{
		if (m_model.Valid())
			RemoveComponents<Model>();
		m_model = component;
	}
	if (typeid(*component) == typeid(Material))
	{
		if (m_material.Valid())
			RemoveComponents<Material>();
		m_material = component;
	}
	if (typeid(*component) == typeid(Transform))
	{
		if (m_transform.Valid())
			RemoveComponents<Transform>();
		m_transform = component;
	}
	if (typeid(*component) == typeid(Rigidbody))
	{
		if (m_rigidbody.Valid())
			RemoveComponents<Rigidbody>();
		m_rigidbody = component;
	}
	m_components.push_back(component);
}
