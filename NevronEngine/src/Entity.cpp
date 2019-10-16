#include <src/pch.h>

#include "Entity.h"
#include <src/Script.h>
#include <src/ResourceManager.h>
#include <Graphics/Renderer.h>

Entity::Entity() : m_name("")
{
	Init("Basic.glsl", "Quad.dae", "Default.mat", 0, Quaternion::identity, 1, {});
}

Entity::Entity(const std::string& shader, const std::string model, const std::string material, std::vector<rsc<Component>> components) : m_name("")
{
	Init(shader, model, material, 0, Quaternion::identity, 1, components);
}

Entity::Entity(const std::string& shader, const std::string model, const std::string material, Vector3 position, Quaternion rotation, Vector3 scale, std::vector<rsc<Component>> components) : m_name("")
{
	Init(shader, model, material, position, rotation, scale, components);
}

void Entity::Update()
{

	for (size_t i = 0; i < m_scripts.size(); i++) m_scripts[i]->Update();
	if (rigidbody)
		rigidbody->Update(transform);
	transform->Update();


}

void Entity::FixedUpdate()
{
	for (size_t i = 0; i < m_scripts.size(); i++) m_scripts[i]->FixedUpdate();
}

void Entity::EarlyUpdate()
{
	for (size_t i = 0; i < m_scripts.size(); i++) m_scripts[i]->EarlyUpdate();
}

void Entity::Render(rsc_weak<Camera> camera)
{
	TransformType t;
	t.mvp = transform->getWorldMatrix() * camera->getViewMatrix() * camera->getProjectionMatrix();

	t.position = transform->position;
	/*t.rotation = transform->rotation;
	t.scale = transform->scale;
	t.camPos = camera->transform.position;
	t.camForward = camera->transform.forward;*/

	shader->Bind();

	shader->setUniformBuffer("Transform", &t, sizeof(TransformType));
	//shader->setUniformMat4f("u_MVP", t.mvp);
	//shader->setUniformMat4f("u_MVP", t.mvp);
	if (!shader || !shader->isValid())
	{
		LogE(m_signature, "Invalid shader");
		return;
	}
	if (!model || !model->isValid())
	{
		LogE(m_signature, "Invalid model");
		return;
	}
	if (!material || !material->isValid())
	{
		LogE(m_signature, "Invalid material, using default");
		material = ResourceManager::Get()->GetMaterial("Default.mat");
		return;
	}
	Renderer::Get()->Draw(shader, model, material);
}

void Entity::AddComponent(rsc<Component> component)
{
	if (!component)
		return;

	if (dynamic_cast<Shader*>(&component))
	{
		if (shader)
			RemoveComponents<Shader>();
		shader = component;
	}
	else if (dynamic_cast<Model*>(&component))
	{
		if (model)
			RemoveComponents<Model>();
		model = component;
	}
	else if (dynamic_cast<Material*>(&component))
	{
		if (material)
			RemoveComponents<Material>();
		material = component;
	}
	else if (dynamic_cast<Transform*>(&component))
	{
		if (transform)
			RemoveComponents<Transform>();
		transform = component;
	}
	else if (dynamic_cast<Rigidbody*>(&component))
	{
		if (rigidbody)
			RemoveComponents<Rigidbody>();
		rigidbody = component;
	}
	else if (dynamic_cast<Script*>(&component))
	{
		m_scripts.push_back(component);
		((Script*)&component)->Init(this);
	}
	m_components.push_back(component);
}

void Entity::RemoveSpecialized(rsc<Component> component)
{
	if (dynamic_cast<Shader*>(&component))
		shader = nullptr;
	else if (dynamic_cast<Model*>(&component))
		model = nullptr;
	else if (dynamic_cast<Material*>(&component))
		material = nullptr;
	else if (dynamic_cast<Transform*>(&component))
		transform = nullptr;
	else if (dynamic_cast<Rigidbody*>(&component))
		rigidbody = nullptr;
	else if (dynamic_cast<Script*>(&component))
	{
		for (int i = 0; i < m_scripts.size(); i++)
			if ((void*)&m_scripts[i] == (void*)&component)
				m_scripts.erase(m_scripts.begin() + i);
	}
}

void Entity::RefreshComponents()
{
	m_scripts.erase(m_scripts.begin(), m_scripts.end());
	for (int i = 0; i < m_components.size(); i++)
	{
		if (dynamic_cast<Shader*>(&m_components[i]))
			shader = m_components[i];

		else if (dynamic_cast<Model*>(&m_components[i]))
			model = m_components[i];

		else if (dynamic_cast<Material*>(&m_components[i]))
			material = m_components[i];

		else if (dynamic_cast<Transform*>(&m_components[i]))
			transform = m_components[i];

		else if (dynamic_cast<Rigidbody*>(&m_components[i]))
			rigidbody = m_components[i];

		else if (dynamic_cast<Script*> (&m_components[i]))
			m_scripts.push_back(m_components[i]);

	}
}

void Entity::Init(const std::string& shader, const std::string& model, const std::string& material, Vector3 position, Quaternion rotation, Vector3 scale, std::vector<rsc<Component>> components)
{
	m_GUID.Generate();
	ResourceManager* rscManager = ResourceManager::Get();
	this->shader = rscManager->GetShader(shader);
	m_components.push_back(this->shader);
	this->model = rscManager->GetModel(model);
	m_components.push_back(this->model);
	AddComponent<Material>(material);
	//this->material = rscManager->GetMaterial(material);
	//m_components.push_back(this->material);

	m_components.insert(m_components.end(), components.begin(), components.end());
	RefreshComponents();

	if (!transform)
		this->transform = new Transform(position, rotation, scale);

	//this->model = Model::GenerateQuad();

	m_signature = "Entity : " + m_name + ";" + m_GUID.getString();
}

