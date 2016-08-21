#include "..\Includes\GameObject.h"
#include "..\Includes\Component.h"

template<class T>
T* component_cast ( GameObject& object )
{
	return object.GetComponent<T> ();
}

template<class T>
T* component_cast ( GameObject* pObject )
{
	T* pComponent = nullptr;
	if ( pObject )
	{
		pComponent = pObject->GetComponent<T> ();
	}

	return pComponent;
}

GameObject::GameObject ()
{}

GameObject::~GameObject ()
{
	for ( ComponentUnorderedMapIterator iter = m_components.begin ();
		iter != m_components.end (); )
	{
		iter->second = nullptr;
	}
}

Component* GameObject::GetComponent ( unsigned short id )
{
	ComponentUnorderedMapIterator result = m_components.find ( id );
	return result == m_components.end () ? nullptr : result->second;
}

bool GameObject::AddComponent ( Component* pComponent )
{
	bool added = false;

	ComponentUnorderedMapIterator result = m_components.find ( pComponent->GetID () );
	if ( result == m_components.end () )
	{
		std::pair<unsigned short, Component*> newComponent ( pComponent->GetID (), pComponent );
		std::pair<ComponentUnorderedMapIterator, bool> addedIter = m_components.insert ( newComponent );

		added = addedIter.second;
	}

	return added;
}
