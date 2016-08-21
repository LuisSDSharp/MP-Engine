#include "..\Includes\WinInput.h"
#include "..\Includes\Platform.h"

#if PLATFORM_WINDOWS

UINT WinInput::m_msg = 0;
WPARAM WinInput::m_wparam = 0;

WinInput::WinInput ( unsigned int priority ) : Input ( priority )
{}

WinInput::~WinInput ()
{
	m_msg = m_wparam = 0;
}

void WinInput::Update ()
{
	m_bWasKeyDown = m_bWasKeyUp = false;

	switch ( m_msg )
	{
		case WM_KEYDOWN:
			m_bWasKeyDown = true;
		break;

		case WM_KEYUP:
			m_bWasKeyUp = true;
		break;

		case WM_DESTROY:
		case WM_CLOSE:
			Platform::SetClosing ();
		break;
	}

	if ( m_bWasKeyDown || m_bWasKeyUp )
	{
		m_KeyCode = m_wparam;
	}

	m_msg = m_wparam = 0;
}

void WinInput::RemapKeyCodes ()
{
	m_Keys.ESCAPE = (KeyCode) VK_ESCAPE;
}

LRESULT WinInput::MessageHandler ( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	switch ( msg )
	{
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_DESTROY:
		case WM_CLOSE:
		{
			m_msg	 = msg;
			m_wparam = wparam;
			return 0;
		}

		default:
			return DefWindowProc ( hwnd, msg, wparam, lparam );
	}
}

#endif
