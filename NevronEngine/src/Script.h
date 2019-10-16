#pragma once
#include <src/Component.h>
#include <src/Entity.h>

//Virtual class describing a script that can be used to maniulate and induce behaviours on entitys
class Script
{
private:
	Entity* m_entity;
public:
	void Init(Entity* entity) { m_entity = entity; }

	virtual void Start() {}
	virtual void Update() {}
	virtual void FixedUpdate() {}
	virtual void EarlyUpdate() {}

	virtual ~Script() {};
protected:
	template <typename R>
	inline void RemoveComponent() { m_entity->RemoveComponent<R>(); };

	template <typename R>
	inline void RemoveComponent(const std::string& name) { m_entity->RemoveComponent(name); }

	template <typename R>
	inline void RemoveComponents() { m_entity->RemoveComponents<R>(); }

	template <typename R>
	inline void RemoveComponents(const std::string& name) { m_entity->RemoveComponents(name); }

	template <typename A>
	inline void AddComponent(rsc<A> component) { m_entity->AddComponent(component); }

	template <typename A>
	inline void AddComponent(const std::string& name) { m_entity->AddComponent(name); }

	template <typename C>
	inline rsc<C> GetComponent() { return m_entity->GetComponent<C>(); }

	template <typename C>
	inline rsc<C> GetComponent(const std::string& name) { return m_entity->GetComponent(name); }
};
