#pragma once
#include <string>
#include <vector>

class String
	: public std::string
{
	std::string Capitalize() const;

	std::string Uncapitalize() const;

	std::string Title() const;

	//Will return a list of all indices of keyW in str
	std::vector<unsigned int> Find(const std::string& keyW) const;

	//Will divide the string at every keyW(default space)
	std::vector<std::string> Split(const std::string& keyW) const;

	std::string Clamp(unsigned int size) const;

	//Will return a string to the keyW
	std::string Stop(std::string keyW) const;

	std::string Purge(const std::string& keyW) const;

	//Will add $lead to string if it doesn't already end with $lead. Good for trailing slashes
	inline std::string Lead(char lead) const;

	//Will add $lead to string if it doesn't already end with $lead. Good for trailing slashes
	inline std::string Lead(const std::string& lead) const;
};




