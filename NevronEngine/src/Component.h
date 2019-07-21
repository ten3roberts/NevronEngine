#pragma once
#include <string>

//This is a base class and is used as a common parent for all Shaders, Textures, Transforms, Scripts and so on that a Object might use. The class itself shall not be used

class Component 
{
public:
	
	//Used to differentiate the type of object if it has several inheritors; like scripts
	enum class Type
	{
		Component, Script
	};
protected:
	std::string m_name;

	//The ID of the resource shared by all references of this component if managed by resource manager, if not value is -1
	//For OpenGL bound classes the rscID is the OpenGL location/id and is not determined by the resource manager
	unsigned int m_rscID;
public:

	const std::string& getName() const { return m_name; };

	//Will return what base type the component is if it has children classes. Scripts will be derived so it's used to keep track of what it is.
	virtual Type getType() const { return Type::Component; }

	/*//Override this function to change if the component in question should be managed
	virtual bool Managed() { return false; }*/

};

/*struct Component
{
	//Intializes resource holder with an empty resource
	Component();
	//Creates a new managed resource
	Component(Resource* rsc);
	//Ref copy
	Component(const Component&);
	
	~Component();

	void makeStrong();
	void makeWeak();
	bool getStrenght() { return strong; }

	Resource* operator->() { return rsc; }
	Resource& operator*() { return *rsc; }

	unsigned int getInstanceCount() const { return *m_referenceCount; }
private:
	Resource* rsc;

	//Determines wether or not this reference should be accounted into the reference count and should actively keep the resource alive.
	bool strong;
	unsigned int* m_referenceCount;
};*/