#include <src/pch.h>
#include "Component.h"
#include <src/ResourceManager.h>

Component::Component() : m_name(""), m_bufferID(-1), m_valid(true)
{
}

Component::Component(const std::string& name) : m_name(name), m_bufferID(-1), m_valid(true)
{
}

Component::~Component()
{
	ResourceManager::Get()->Refresh();
}
