#include "Component.h"
#include <src/ResourceManager.h>

Component::~Component()
{
	ResourceManager::Get()->Refresh();
}
