#pragma once
#include <src/pch.h>
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
	bool m_valid;
private:
	GUID m_GUID;
public:

	const std::string& getName() const { return m_name; };
	GUID getGUID() const { return m_GUID; }
	unsigned int getBufferID() const { return m_bufferID; }

	//Will return what base type the component is if it has children classes. Scripts will be derived so it's used to keep track of what it is.
	virtual Type getType() const { return Type::Component; }
	bool getValid() { return m_valid; }
	/*//Override this function to change if the component in question should be managed
	virtual bool Managed() { return false; }*/

};