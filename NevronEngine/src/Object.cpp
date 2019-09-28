#include "Object.h"
#include <src/ResourceManager.h>
#include <Graphics/Renderer.h>

Object::Object()
{
}

Object::Object(const std::string& shader, const std::string model, const std::string material, std::vector<rsc<Component>> components)
{
	Init(shader, model, material, 0, Quaternion::identity, 1, components);
}

Object::Object(const std::string& shader, const std::string model, const std::string material, Vector3 position, Quaternion rotation, Vector3 scale, std::vector<rsc<Component>> components)
{
	Init(shader, model, material, position, rotation, scale, components);
}

void Object::Update()
{
	if (rigidbody)
		rigidbody->Update(transform);
	transform->Update();
}

void Object::FixedUpdate()
{
}

void Object::Render(rsc_weak<Camera> camera)
{
	TransformType t;
	Update();
	t.mvp = transform->getWorldMatrix() * camera->getViewMatrix() * camera->getProjectionMatrix();

	t.position = transform->position;
	/*t.rotation = transform->rotation;
	t.scale = transform->scale;
	t.camPos = camera->transform.position;
	t.camForward = camera->transform.forward;*/

	shader->Bind();

	shader->setUniformBuffer("Transform", &t, sizeof(TransformType));
	//shader->setUniformMat4f("u_MVP", t.mvp);

	Renderer::Get()->Draw(shader, model, material);
}

void Object::AddComponent(rsc<Component> component)
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
	m_components.push_back(component);
}

void Object::RemoveSpecialized(rsc<Component> component)
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
}

void Object::RefreshComponents()
{
	for (int i = 0; i < m_components.size(); i++)
	{
		if (dynamic_cast<Shader*>(&m_components[i]))
		{
			shader = m_components[i];
		}
		else if (dynamic_cast<Model*>(&m_components[i]))
		{
			model = m_components[i];
		}
		else if (dynamic_cast<Material*>(&m_components[i]))
		{
			material = m_components[i];
		}
		else if (dynamic_cast<Transform*>(&m_components[i]))
		{
			transform = m_components[i];
		}
		else if (dynamic_cast<Rigidbody*>(&m_components[i]))
		{
			rigidbody = m_components[i];
		}
	}
}

void Object::Init(const std::string& shader, const std::string& model, const std::string& material, Vector3 position, Quaternion rotation, Vector3 scale, std::vector<rsc<Component>> components)
{
	ResourceManager* rscManager = ResourceManager::Get();
	this->shader = rscManager->GetShader(shader);
	m_components.push_back(this->shader);
	this->model = rscManager->GetModel(model);
	m_components.push_back(this->model);
	this->material = rscManager->GetMaterial(material);
	m_components.push_back(this->material);

	m_components.insert(m_components.end(), components.begin(), components.end());
	RefreshComponents();

	if (!transform)
		this->transform = new Transform(position, rotation, scale);

	this->model = Model::GenerateQuad();
}

