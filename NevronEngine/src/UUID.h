#pragma once
#include <string>
#include <bitset>
class UUID
{
private:
	std::string m_value;
public:
	UUID();
	void Generate();
	operator std::string()
	{
		return m_value;
	}
	//Generates a UUID complient with the RFC 4122 standard
	std::string getString() const { return m_value; }
};

