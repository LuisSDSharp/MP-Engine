#include "..\Includes\InputManager.h"
#include "..\Includes\Input.h"

InputManager::InputManager ( Input* const pInput ) : ISingleton (), m_owner ( pInput )
{}

InputManager::~InputManager ()
{
	m_owner = NULL;
}

bool InputManager::IsKeyUp ( KeyCode pKey )
{
	return m_owner->IsKeyUp ( pKey );
}

bool InputManager::IsKeyDown ( KeyCode pKey )
{
	return m_owner->IsKeyDown ( pKey );
}

KeyCodes InputManager::KBKeys ()
{
	return m_owner->KeyCodes ();
}
