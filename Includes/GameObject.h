#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

class Component;

#ifndef NDEBUG
#include "Logger.h"
#endif

#include <unordered_map>

class GameObject
{
	template<class T>
	friend T* component_cast ( GameObject& object );

	template<class T>
	friend T* component_cast ( GameObject* pObject );

private:
	typedef std::tr1::unordered_map<unsigned int, Component*> ComponentUnorderedMap;
	typedef ComponentUnorderedMap::iterator ComponentUnorderedMapIterator;

	ComponentUnorderedMap m_components;

	Component* GetComponent ( unsigned short id );

protected:
	GameObject ();
	~GameObject ();

	bool AddComponent ( Component* );
};

#endif