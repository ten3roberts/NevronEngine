#include "XMLParser.h"
#include <src/pch.h>

using namespace Utility;

XMLNode::XMLNode(const std::string& filename) : m_tag(""), m_content(""), m_children(), m_depth(0), m_parent(nullptr), m_valid(true)
{
	m_filepath = FindFile(filename);
	std::ifstream file(m_filepath);
	std::string tmp, file_cont = "";
	if (!file.is_open())
	{
		m_valid = false; return;
	}
	while (std::getline(file, tmp)) { file_cont += tmp; }
	if (!file_cont.size()) { m_valid = false; return; }
	Load(file_cont);
}


/*XMLNode::XMLNode(const XMLNode& node)
{
	m_attributes = node.m_attributes;
	m_children = node.m_children;
	m_parent = node.m_parent;
	m_depth = node.m_depth;
	m_tag = node.m_tag;
}*/

XMLNode::~XMLNode()
{
	for (int i = 0; i < m_children.size(); i++)
	{
		std::vector<XMLNode*>& children = *getChildren(i);
		for (int j = 0; j < children.size(); j++)
		{
			delete children[j];
		}
	}
}

std::string XMLNode::Load(std::string str)
{
	///Removes unnecessary new line and tab characters
	str = strPurgeAll(str, "\n\t");
	std::string org = str;
	//Finds the beginning of the opening tag
	size_t tag_open;

	//Skipping processing instructions
	while (true)
	{
		tag_open = str.find('<');
		if (str[tag_open + 1] == '?')
		{
			str = str.substr(tag_open + 2);
		}
		else
			break;

		if (tag_open == std::string::npos)
			return str;
	}

	str = str.substr(tag_open + 1);
	if (tag_open == std::string::npos)
		return str;

	//Finds the end of the opening tag
	size_t tag_end = str.find('>');

	//Divides the inside of the tag into several parts. [0] is the tag name. The other optional ones are attributes
	std::vector<std::string> tag_parts = strSplit(str.substr(0, tag_end), ' ', true);
	if (!tag_parts.size())
		return str;
	m_tag = tag_parts[0];
	if (!m_tag.size())
		return "";


	if (tag_parts.size() > 1)
	{
		for (size_t i = 1; i < tag_parts.size(); i++)
		{
			tag_parts[i] = strPurge(tag_parts[i], '"');
			size_t attrib_equals = tag_parts[i].find('=');
			std::string attrib_name = tag_parts[i].substr(0, attrib_equals);
			std::string attrib_value = tag_parts[i].substr(attrib_equals + 1);
			m_attributes[attrib_name] = attrib_value;
		}
	}

	//Indicates the beginning of the closing tag
	if (str[tag_end - 1] == '/') //Node is empty
	{
		m_content = "";
		return org.substr(0, tag_open) + str.substr(tag_end+1);
	}
	size_t close_tag = str.find("</" + m_tag + ">");
	if (close_tag == std::string::npos)
		return "";
	//Find the body between the opening and closing tag
	std::string body = str.substr(tag_end + 1, close_tag - tag_end - 1);


	//Find the children inside of the body. Children returns the part after them
	//Breaks the loop when there are no more childs in the body
	while (body.size())
	{
		XMLNode* child = new XMLNode(this, m_depth + 1, m_valid);
		std::string tmp(child->Load(body));
		if (tmp == body)
		{
			break;
		}
		body = tmp;


		//Checks if the child was valid
		if (!child->getTag().size())
			continue;
		m_children[child->getTag()].push_back(child);
	}
	m_content = body;

	return org.substr(0, tag_open) + str.substr(close_tag + m_tag.size() + 3);
}

std::string XMLNode::GenerateString()
{
	std::string tabs;
	tabs.insert(tabs.begin(), m_depth, '\t');
	std::string attributes;
	auto it = m_attributes.begin();
	for (auto it = m_attributes.begin(); it != m_attributes.end(); it++)
	{
		attributes += ' ' + it->first + "=\"" + it->second + '"';
	}
	std::string body = '<' + m_tag + attributes + '>';
	if (m_content.size() && m_children.size())
		body += "\n" + tabs + '\t' + m_content + '\n';
	else if (m_content.size())
		body += m_content;

	//Generating all the childs string
	for (int i = 0; i < m_children.size(); i++)
	{

		std::vector<XMLNode*>& children = *getChildren(i);
		for (int j = 0; j < children.size(); j++)
		{
			if (i > 0 || j > 0)
				body += '\n';
			const std::string& tmp = children[j]->GenerateString();
			body += (tmp.find('\n') != std::string::npos || m_children.size() > 1 || m_content.size() ? tabs + '\t' : "") + tmp;
		}
	}

	if (m_children.size() && m_content.size())
		body += '\n' + tabs;
	body += "</" + m_tag + '>';

	return body;
}

void XMLNode::Save(std::string filepath)
{
	GenerateFile(filepath, GenerateString(), false);
}

void XMLNode::Save()
{
	GenerateFile(m_filepath, GenerateString(), false);
}

XMLNode* XMLNode::Find(const std::string& tag)
{
	if (m_tag == tag)
		return this;

	//Generating all the childs string
	XMLNode* result = nullptr;
	for (int i = 0; i < m_children.size(); i++)
	{

		std::vector<XMLNode*>& children = *getChildren(i);
		for (int j = 0; j < children.size(); j++)
		{
			result = children[j]->Find(tag);
			if (result != nullptr)
				return result;
		}
	}
	return nullptr;
}

