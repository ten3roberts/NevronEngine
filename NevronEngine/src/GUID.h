#pragma once
#include <src/pch.h>

#include <bitset>
class GUID
{
private:
	std::string m_value;
public:
	GUID();
	void Generate();
	operator std::string()
	{
		return m_value;
	}

	bool operator==(const GUID& ID)
	{
		return ID.getString() == m_value;
	}
	//Generates a GUID complient with the RFC 4122 standard
	std::string getString() const { return m_value; }
};

