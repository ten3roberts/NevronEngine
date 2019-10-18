#pragma once
#include <string>
#include <unordered_map>
#include <src/pch.h>

class XMLNode
{
	std::string m_filepath;
	std::string m_tag;
	std::string m_content;
	XMLNode* m_parent;
	bool m_valid;
	std::unordered_map<std::string, std::vector<XMLNode*>> m_children;
	std::unordered_map<std::string, std::string> m_attributes;
	size_t m_depth;
public:
	XMLNode(const std::string& filename);
	XMLNode(XMLNode* parent, size_t depth, bool valid) : m_tag(""), m_content(""), m_children(), m_depth(depth), m_parent(parent), m_valid(parent->isValid()) {}
	XMLNode() : m_tag(""), m_content(""), m_children(), m_depth(0), m_parent(nullptr), m_valid(false) {}
	//XMLNode(const XMLNode& node);
	~XMLNode();
	//Loads the xml part and returns what's left
	std::string Load(std::string str);
	std::string GenerateString();
	//Saves the XML to specified file
	void Save(std::string filepath = "");
	void Save();
	std::string getTag() { return m_tag; }
	std::string getContent() { return m_content; }
	std::unordered_map<std::string, std::vector<XMLNode*>>& getChildren() { return m_children; }
	std::vector<XMLNode*>* getChildren(std::string tag) { return &m_children[tag]; }
	std::vector<XMLNode*>* getChildren(size_t index) { auto it = m_children.begin(); std::advance(it, index); return &it->second; }
	std::string getAttribute(const std::string& name) { return m_attributes[name]; }

	size_t getDepth() { return m_depth; }
	std::string getFilepath() { return m_filepath; };
	void setFilepath(const std::string filepath) { m_filepath = filepath; }


	XMLNode* Find(const std::string& tag);
	XMLNode* getParent() { return m_parent; }

	//operator[] returns by reference and not pointer so it can be stacked [][][]
	//XMLNode* operator[](const std::string& tag) { if (m_children.find(tag) != m_children.end()) return m_children[tag][0]; return nullptr; }
	XMLNode& operator[](const std::string& tag) { if (m_children.find(tag) != m_children.end()) return *m_children[tag][0]; m_children[tag] = { new XMLNode(this, m_depth + 1, m_valid) }; return *m_children[tag][0]; }
	XMLNode* operator[](size_t index) { auto it = m_children.begin(); std::advance(it, index); return it->second[0]; }
	bool isValid() { return m_valid; }
};
