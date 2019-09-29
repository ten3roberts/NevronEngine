#pragma once
#include <src/pch.h>
#include <src/Logger.h>

template <typename R>
class rsc_weak;
template <typename R>
class rsc
{
	template<typename U> friend class rsc_weak;
	template<typename U> friend class rsc;
protected:
	R* m_pData;
	unsigned int* m_refCount;
	unsigned int* m_weakRefCount;
private:
	void Remove()
	{
		if (!m_refCount || !m_weakRefCount)
			return;

		(*m_refCount)--;
		//No references left

		if (*m_refCount <= 0 && *m_weakRefCount <= 0)
		{
			delete m_refCount;
			m_refCount = nullptr;

			delete m_weakRefCount;
			m_weakRefCount = nullptr;
			delete m_pData;
			m_pData = nullptr;
			return;
		}

		if (*m_refCount == 0)// && * m_weakRefCount == 0)
		{
			delete m_pData;
			m_pData = nullptr;
		}
		
	}
public:
	operator bool() const { if (m_pData && m_refCount && m_weakRefCount) return true; return false; }

	R* operator->() { return m_pData; }
	R* operator&() { return m_pData; }
	R operator*() { return *m_pData; }


	int getRefCount() { return *m_refCount; }
	int getWeakRefCount() { return *m_weakRefCount; }

	~rsc() { Remove(); }
	rsc() : m_pData(nullptr), m_refCount(nullptr), m_weakRefCount(nullptr) {};

	//Constructor from pointer
	rsc(R* const pData) : m_pData(pData), m_refCount(nullptr), m_weakRefCount(nullptr)
	{
		if (m_pData)
		{
			m_refCount = new unsigned int(1);
			m_weakRefCount = new unsigned int(0);
		}
	}

	//Conversion from pointer
	template <typename C>
	rsc(C* const pData) : m_pData(nullptr), m_refCount(nullptr), m_weakRefCount(nullptr)
	{
		Remove();
		m_pData = dynamic_cast<R*>(pData);
		if (!m_pData && pData)
		{
			LogS("rsc", "Couldn't convert from type %c to %c", typeid(pData).name(), typeid(R*).name());
			m_refCount = nullptr;
			m_weakRefCount = nullptr;
			return;
		}
		if (m_pData)
		{
			m_refCount = new unsigned(1);
			m_weakRefCount = new unsigned int(0);
		}
	}

	//Copy from weak
	rsc(const rsc_weak<R>& resource) : m_pData(resource.m_pData), m_refCount(resource.m_refCount), m_weakRefCount(resource.m_weakRefCount)
	{
		if (m_refCount)
		{
			(*m_refCount)++;
		}
	}

	//Conversion from weak
	template <typename C>
	rsc(const rsc_weak<C>& resource) : m_refCount(resource.m_refCount), m_weakRefCount(resource.m_weakRefCount)
	{
		m_pData = dynamic_cast<R*>(resource.m_pData);

		if (!m_pData && resource.m_pData)
		{
			LogS("rsc", "Couldn't convert from type %c to %c", typeid(resource.m_pData).name(), typeid(R*).name());
			m_refCount = nullptr;
			m_weakRefCount = nullptr;
			return;
		}

		if (m_refCount)
		{
			(*m_refCount)++;
		}
	}

	//Copy from strong
	rsc(const rsc<R>& resource) : m_pData(resource.m_pData), m_refCount(resource.m_refCount), m_weakRefCount(resource.m_weakRefCount)
	{
		if (m_refCount)
		{
			(*m_refCount)++;
		}
	}

	//Conversion from strong
	template <typename C>
	rsc(const rsc<C>& resource) : m_refCount(resource.m_refCount), m_weakRefCount(resource.m_weakRefCount)
	{
		m_pData = dynamic_cast<R*>(resource.m_pData);
		if (!m_pData && resource.m_pData)
		{
			LogS("rsc", "Couldn't convert from type %c to %c", typeid(resource.m_pData).name(), typeid(R*).name());
			m_refCount = nullptr;
			m_weakRefCount = nullptr;
			return;
		}
		if (m_refCount)
		{
			(*m_refCount)++;
		}
	}

	//Assign from weak
	void operator=(const rsc_weak<R>& resource)
	{
		Remove();
		m_pData = resource.m_pData;
		m_refCount = resource.m_refCount;
		m_weakRefCount = resource.m_weakRefCount;
		if (m_refCount)
			(*m_refCount)++;
	}

	//Conversion assign from weak
	template <typename C>
	void operator=(const rsc_weak<C>& resource)
	{
		Remove();
		m_pData = dynamic_cast<R*>(resource.m_pData);

		if (!m_pData && resource.m_pData)
		{
			LogS("rsc", "Couldn't convert from type %c to %c", typeid(resource.m_pData).name(), typeid(R*).name().name());
			m_refCount = nullptr;
			m_weakRefCount = nullptr;
			return;
		}

		m_refCount = resource.m_refCount;
		m_weakRefCount = resource.m_weakRefCount;
		if (m_refCount)
			(*m_refCount)++;

	}

	//Assign from strong
	void operator=(const rsc<R>& resource)
	{
		Remove();
		m_pData = resource.m_pData;
		m_refCount = resource.m_refCount;
		m_weakRefCount = resource.m_weakRefCount;
		if (m_refCount)
			(*m_refCount)++;

	}

	//Conversion assign from strong
	template <typename C>
	void operator=(const rsc<C>& resource)
	{
		Remove();
		m_pData = dynamic_cast<R*>(resource.m_pData);

		if (!m_pData && resource.m_pData)
		{
			LogS("rsc", "Couldn't convert from type %c to %c", typeid(resource.m_pData).name(), typeid(R*).name());
			m_refCount = nullptr;
			m_weakRefCount = nullptr;
			return;
		}

		m_refCount = resource.m_refCount;
		m_weakRefCount = resource.m_weakRefCount;
		if (m_refCount)
			(*m_refCount)++;

	}

	//Assign from pointer
	void operator=(R* const pData)
	{
		Remove();
		m_pData = pData;
		m_refCount = nullptr;
		m_weakRefCount = nullptr;
		if (m_pData)
		{
			m_refCount = new unsigned(1);
			m_weakRefCount = new unsigned int(0);
		}
	}

	//Conversion assign from pointer
	template <typename C>
	void operator=(C* const pData)
	{
		Remove();
		m_pData = dynamic_cast<R*>(pData);

		if (!m_pData && pData)
		{
			LogS("rsc", "Couldn't convert from type %c to %c", typeid(pData).name(), typeid(R*).name().name());
			m_refCount = nullptr;
			m_weakRefCount = nullptr;
			return;
		}

		m_refCount = nullptr;
		m_weakRefCount = nullptr;
		if (m_pData)
		{
			m_refCount = new unsigned(1);
			m_weakRefCount = new unsigned int(0);
		}
	}
};

template <typename W>
class rsc_weak
{
	template<typename U> friend class rsc_weak;
	template<typename U> friend class rsc;
protected:
	W* m_pData;
	unsigned int* m_refCount;
	unsigned int* m_weakRefCount;
private:
	void Remove()
	{
		if (!m_refCount || !m_weakRefCount)
			return;

		(*m_weakRefCount)--;
		//No references left
		if (*m_refCount <= 0 && *m_weakRefCount <= 0)
		{
			delete m_refCount;
			m_refCount = nullptr;
			delete m_weakRefCount;
			m_weakRefCount = nullptr;
		}
	}
public:
	operator bool() const { if (m_pData && m_refCount && m_weakRefCount) return true; return false; }

	W* operator->() { return m_pData; }
	W* operator&() { return m_pData; }
	W operator*() { return *m_pData; }

	int getRefCount() { return *m_refCount; }
	int getWeakRefCount() { return *m_weakRefCount; }

	~rsc_weak() { Remove(); }

	rsc_weak() : m_pData(nullptr), m_refCount(nullptr), m_weakRefCount(nullptr) {};

	//Constructor from pointer
	rsc_weak(W* const pData) : m_pData(pData), m_refCount(nullptr), m_weakRefCount(nullptr)
	{
		if (m_pData)
		{
			m_refCount = new unsigned int(0);
			m_weakRefCount = new unsigned int(1);
		}
	}

	//Conversion from pointer
	template <typename C>
	rsc_weak(C* const pData) : m_pData(nullptr), m_refCount(nullptr), m_weakRefCount(nullptr)
	{
		Remove();
		m_pData = dynamic_cast<W*>(pData);

		if (!m_pData && pData)
		{
			LogS("rsc", "Couldn't convert from type %c to %c", typeid(pData).name(), typeid(W*).name());
			m_refCount = nullptr;
			m_weakRefCount = nullptr;
			return;
		}

		if (m_pData)
		{
			m_refCount = new unsigned(0);
			m_weakRefCount = new unsigned int(1);
		}
	}

	//Copy from weak
	rsc_weak(const rsc_weak<W>& resource) : m_pData(resource.m_pData), m_refCount(resource.m_refCount), m_weakRefCount(resource.m_weakRefCount)
	{
		if (m_weakRefCount)
		{
			(*m_weakRefCount)++;
		}
	}

	//Conversion from weak
	template <typename C>
	rsc_weak(const rsc_weak<C>& resource) : m_refCount(resource.m_refCount), m_weakRefCount(resource.m_weakRefCount)
	{
		m_pData = dynamic_cast<W*>(resource.m_pData);

		if (!m_pData && resource.m_pData)
		{
			LogS("rsc", "Couldn't convert from type %c to %c", typeid(resource.m_pData).name(), typeid(W*).name());
			m_refCount = nullptr;
			m_weakRefCount = nullptr;
			return;
		}

		if (m_weakRefCount)
		{
			(*m_weakRefCount)++;
		}
	}

	//Copy from strong
	rsc_weak(const rsc<W>& resource) : m_pData(resource.m_pData), m_refCount(resource.m_refCount), m_weakRefCount(resource.m_weakRefCount)
	{
		if (m_weakRefCount)
		{
			(*m_weakRefCount)++;
		}
	}

	//Conversion from strong
	template <typename C>
	rsc_weak(const rsc<C>& resource) : m_refCount(resource.m_refCount), m_weakRefCount(resource.m_weakRefCount)
	{
		m_pData = dynamic_cast<W*>(resource.m_pData);
		if (!m_pData && resource.m_pData)
		{
			LogS("rsc", "Couldn't convert from type %c to %c", typeid(resource.m_pData).name(), typeid(W*).name());
			m_refCount = nullptr;
			m_weakRefCount = nullptr;
			return;
		}
		if (m_weakRefCount)
		{
			(*m_weakRefCount)++;
		}
	}

	//Assign from weak
	void operator=(const rsc_weak<W>& resource)
	{
		Remove();
		m_pData = resource.m_pData;
		m_refCount = resource.m_refCount;
		m_weakRefCount = resource.m_weakRefCount;
		if (m_weakRefCount)
			(*m_weakRefCount)++;
	}

	//Conversion assign from weak
	template <typename C>
	void operator=(const rsc_weak<C>& resource)
	{
		Remove();
		m_pData = dynamic_cast<W*>(resource.m_pData);
		if (!m_pData && resource.m_pData)
		{
			LogS("rsc", "Couldn't convert from type %c to %c", typeid(resource.m_pData).name(), typeid(W*).name());
			m_refCount = nullptr;
			m_weakRefCount = nullptr;
			return;
		}
		m_refCount = resource.m_refCount;
		m_weakRefCount = resource.m_weakRefCount;
		if (m_weakRefCount)
			(*m_weakRefCount)++;

	}

	//Assign from strong
	void operator=(const rsc<W>& resource)
	{
		Remove();
		m_pData = resource.m_pData;
		m_refCount = resource.m_refCount;
		m_weakRefCount = resource.m_weakRefCount;
		if (m_weakRefCount)
			(*m_weakRefCount)++;

	}

	//Conversion assign from strong
	template <typename C>
	void operator=(const rsc<C>& resource)
	{
		Remove();
		m_pData = dynamic_cast<W*>(resource.m_pData);
		if (!m_pData && resource.m_pData)
		{
			LogS("rsc", "Couldn't convert from type %c to %c", typeid(resource.m_pData).name(), typeid(W*).name());
			m_refCount = nullptr;
			m_weakRefCount = nullptr;
			return;
		}
		m_refCount = resource.m_refCount;
		m_weakRefCount = resource.m_weakRefCount;
		if (m_weakRefCount)
			(*m_weakRefCount)++;

	}

	//Assign from pointer
	void operator=(W* const pData)
	{
		Remove();
		m_pData = pData;
		m_refCount = nullptr;
		m_weakRefCount = nullptr;
		if (m_pData)
		{
			m_refCount = new unsigned(0);
			m_weakRefCount = new unsigned int(1);
		}
	}

	//Conversion assign from pointer
	template <typename C>
	void operator=(C* const pData)
	{
		Remove();
		m_pData = dynamic_cast<W*>(pData);
		if (!m_pData && pData)
		{
			LogS("rsc", "Couldn't convert from type %c to %c", typeid(m_pData).name(), typeid(W*).name());
			m_refCount = nullptr;
			m_weakRefCount = nullptr;
			return;
		}
		m_refCount = nullptr;
		m_weakRefCount = nullptr;
		if (m_pData)
		{
			m_refCount = new unsigned(0);
			m_weakRefCount = new unsigned int(1);
		}
	}
};