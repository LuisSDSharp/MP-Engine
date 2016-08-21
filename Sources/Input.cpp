#include "..\Includes\Input.h"
#include "..\Includes\InputManager.h"

Input::Input(unsigned int priority) : ITask(priority)
{
	m_inputManager = new InputManager ( this );
}

Input::~Input()
{
	delete m_inputManager;
	m_inputManager = NULL;
}

bool Input::Start()
{
	Log::Info ( "Input taks started" );

	RemapKeyCodes();
	return true;
}
