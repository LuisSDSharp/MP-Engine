#ifndef INPUT_MANAGER_H_
#define INPUT_MANAGER_H_

class Input;

#include "ISingleton.h"
#include "Input.h"

class InputManager : public ISingleton<InputManager>
{
	friend class Input;

private:
	InputManager ( Input* const );
	~InputManager ();

	Input*			m_owner;

public:
	bool			IsKeyUp ( KeyCode );
	bool			IsKeyDown ( KeyCode );
	KeyCodes		KBKeys ();
};

#endif
