#include "String.h"

std::string String::Capitalize() const
{
	std::string result(size(), ' ');
	for (unsigned int i = 0; i < size(); i++)
	{
		result[i] = toupper((*this)[i]);
	}
	return result;
}

std::string String::Uncapitalize() const
{
	std::string result(size(), ' ');
	for (unsigned int i = 0; i < size(); i++)
	{
		result[i] = tolower((*this)[i]);
	}
	return result;
}

std::string String::Title() const
{
	std::string result = *this;
	result[0] = toupper((*this)[0]);
	return result;
}

std::vector<unsigned int> String::Find(const std::string& keyW) const
{
	std::vector<unsigned int> results;

	for (unsigned int i = 0; i < size() - (keyW.size() - 1); i++)
	{
		if (substr(i, keyW.size()) == keyW)
		{
			results.push_back(i);
		}
	}
	return results;
}

std::vector<std::string> String::Split(const std::string& keyW) const
{
	//Terminating string with keyW
		std::string text = *this;
	if (!(text.substr(text.size() - keyW.size(), keyW.size()) == keyW))
		text += keyW;

	std::vector<std::string> results;
	for (unsigned int i = 0; i < text.size(); i++)
	{
		if (text.substr(i, keyW.size()) == keyW) //Cursor att keyword
		{
			results.push_back(text.substr(0, i)); //Push left bit to results
			text = text.substr(i + 1); //And remove it from string and reset cursor
			i = 0;
		}
	}

	return results;
}

std::string String::Clamp(unsigned int size) const
{
	return substr(0, size);
}

std::string String::Stop(std::string keyW) const
{
	return substr(find_first_of(keyW));
}

std::string String::Purge(const std::string& keyW) const
{
	std::string result;
	//Left part iterator
	unsigned int c = 0;
	for (unsigned int i = 0; i < size(); i++)
	{
		if (substr(i, keyW.size()) == keyW) //Adding left part of string
		{
			result += substr(c, i - c);

			//Skipping past $keyW
			i += (unsigned int)keyW.size();

			//Cathing up with left iterator
			c = i;
		}
	}
	return result + substr(c);
}

inline std::string String::Lead(char lead) const
{
		if (back() == lead)
			return *this;
		return *this + lead;
}

inline std::string String::Lead(const std::string& lead) const
{
	if (lead.size() <= size() && substr(size() - lead.size()) == lead)
		return *this;
	return *this + lead;
}


