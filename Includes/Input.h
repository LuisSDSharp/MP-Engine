#ifndef INPUT_H_
#define INPUT_H_

class InputManager;

#include "Platforms.h"
#include "ITask.h"

#ifndef NDEBUG
#include "Logger.h"
#endif

#if PLATFORM_ANDROID
#include <stdint.h>

typedef int32_t KeyCode;
#elif PLATFORM_WINDOWS
typedef __int32 KeyCode;
#endif

struct KeyCodes
{
	KeyCode ESCAPE;
	KeyCode ARROW_UP;
	KeyCode ARROW_DOWN;
	KeyCode ARROW_LEFT;
	KeyCode ARROW_RIGHT;
};

class Input : public ITask
{
private:
	InputManager*				m_inputManager;

protected:
	bool m_bWasKeyUp;
	bool m_bWasKeyDown;
	KeyCode m_KeyCode;

	KeyCodes m_Keys;

public:
	Input ( const unsigned int priority );
	virtual ~Input ();

	KeyCodes					KeyCodes () { return m_Keys; }

	virtual bool				Start ();
	virtual void				OnSuspend () { LOG_FUNCTION_NOT_IMPLEMENTED; }
	virtual void				Update () {}
	virtual void				OnResume () { LOG_FUNCTION_NOT_IMPLEMENTED; }
	virtual void				Stop () { Log::Info ( "Input taks stoped" ); }

	virtual bool				IsKeyDown ( KeyCode keyCode ) { return ( m_bWasKeyDown && keyCode == m_KeyCode ) ? true : false; }
	virtual bool				IsKeyUp ( KeyCode keyCode ) { return ( m_bWasKeyUp && keyCode == m_KeyCode ) ? true : false; }

	virtual void				RemapKeyCodes () = 0;
};

#endif