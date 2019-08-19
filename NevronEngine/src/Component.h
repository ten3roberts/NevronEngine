#pragma once
#include <string>
#include <src/Logger.h>
#include <src/GUID.h>

class Shader;
class Model;
class Material;
class Texture;
class UniformBuffer;


//This is a base class and is used as a common parent for all Shaders, Textures, Transforms, Scripts and so on that a Object might use. The class itself shall not be used
class Component
{
public:
	Component();
	Component(const std::string& name);
	~Component();
	//Used to differentiate the type of object if it has several inheritors; like scripts
	enum class Type
	{
		Component, Script
	};
protected:
	std::string m_name;

	//The ID of the resource of this component if managed by resource manager, if not value is -1 or UINT32_MAX
	//For OpenGL bound classes; the id of the buffer or program on the GPU
	unsigned int m_bufferID;
private:
	GUID m_GUID;
public:

	const std::string& getName() const { return m_name; };
	GUID getGUID() const { return m_GUID; }
	unsigned int getBufferID() const { return m_bufferID; }

	//Will return what base type the component is if it has children classes. Scripts will be derived so it's used to keep track of what it is.
	virtual Type getType() const { return Type::Component; }

	/*//Override this function to change if the component in question should be managed
	virtual bool Managed() { return false; }*/

};

template <typename R, bool Strong = true>
struct rsc
{
	rsc() : m_pData(nullptr), m_referenceCount(nullptr), m_strong(Strong) {}
	//Creates a new managed resource
	rsc(R* pData) : m_pData(pData), m_referenceCount(nullptr), m_strong(Strong)
	{
		if (m_pData)
			m_referenceCount = new unsigned int(m_strong);
	}

	//Enables the instance to be compared with a bool; returns true if m_pData is valid
	operator bool() const
	{
		return Valid();
	}

	template <typename A, bool S>
	//Assigns a new internal pointer and removes one instance from the previous one. The pointer keeps its strength
	void operator=(const rsc<A, S>& rsc)
	{
		Remove();
		m_pData = dynamic_cast<R*>(rsc.GetPointer());
		if (!m_pData)
		{
			LogS("rsc", "Couldn't convert resource; Incompatible types");
			return;
		}
		m_referenceCount = rsc.getRawReferenceCount();
		if (m_strong)
			(*m_referenceCount)++;
	}

	void operator=(const rsc<R, Strong>& rsc)
	{
		Remove();
		if (rsc.GetPointer())
		{
			m_pData = dynamic_cast<R*>(rsc.GetPointer());
			if (!m_pData)
			{
				if (rsc)
					LogS("rsc", "Couldn't convert resource; Incompatible types");
				return;
			}
		}
		m_referenceCount = rsc.getRawReferenceCount();
		if (m_strong)
			(*m_referenceCount)++;
	}

	//Ref copy from other pointer
	template<typename D = R, bool S>
	rsc(const rsc<D, S>& rsc) : m_pData(nullptr), m_referenceCount(nullptr), m_strong(Strong)
	{
		m_pData = dynamic_cast<R*>(rsc.GetPointer());
		if (!m_pData)
		{
			if (rsc)
				LogS("rsc", "Couldn't convert resource; Incompatible types");
			return;
		}
		m_referenceCount = rsc.getRawReferenceCount();
		(*m_referenceCount)++;
	}

	//Ref copy
	rsc(const rsc<R, Strong>& rsc) : m_strong(Strong)
	{
		m_pData = rsc.m_pData;
		m_referenceCount = rsc.m_referenceCount;
		(*m_referenceCount)++;
	}

	~rsc()
	{
		Remove();
	}

	void makeStrong()
	{
		if (m_strong) //If already strong
			return;
		(*m_referenceCount)++;
		m_strong = true;
	}

	void makeWeak()
	{
		if (!m_strong) //If already weak
			return;
		(*m_referenceCount)--;
		m_strong = false;

	}

	void setStrength(bool strong)
	{
		if (strong)
			makeStrong();
		else
			makeWeak();
	}

	bool getStrength() const { return m_strong; }

	R* operator->() const { return m_pData; }	//Dereference
	R* operator&() { return m_pData; }	//Returns internal pointer
	R& operator*() { return *m_pData; } //Returns reference to *pData


	unsigned int getReferenceCount() const { return *m_referenceCount; }
	unsigned int* getRawReferenceCount() const { return m_referenceCount; }
	R* GetPointer() const { return m_pData; }

	inline bool Valid() const { if (m_pData && m_referenceCount) return true; return false; }

private:
	R* m_pData;

	//Determines whether or not this reference should be accounted into the reference count and should actively keep the resource alive.
	bool m_strong;
	unsigned int* m_referenceCount;

	void Remove()
	{
		if (!Valid()) return;
		if (!m_strong && m_referenceCount && *m_referenceCount == 0)
		{
			//LogS("rsc", "Last one alive");
			delete m_referenceCount;
			return;
		}

		if (m_strong && *m_referenceCount == 1)
		{
			(*m_referenceCount)--;
			if (m_referenceCount)
			{
				m_referenceCount = nullptr;
				delete m_referenceCount;
			}
			if (m_pData)
			{
				m_pData = nullptr;
				delete m_pData;
			}

		}
		else if (m_strong)
			(*m_referenceCount)--;
	}
};