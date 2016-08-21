#include "../Includes/WinPlatform.h"
#include "../Includes/WinInput.h"

#if PLATFORM_WINDOWS

HWND WinPlatform::m_hwnd = NULL;

WinPlatform::WinPlatform ( unsigned int priority ) : Platform ( priority )
{}

WinPlatform::~WinPlatform ()
{}

bool WinPlatform::Start ()
{
	WNDCLASSEX wc;
	//DEVMODE dmScreenSettings;
	int posX, posY;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle ( NULL );

	// Give the application a name.
	m_applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon ( NULL, IDI_WINLOGO );
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor ( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH) GetStockObject ( BLACK_BRUSH );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof ( WNDCLASSEX );

	// Register the window class.
	RegisterClassEx ( &wc );

	// Place the window in the middle of the screen.
	posX = ( GetSystemMetrics ( SM_CXSCREEN ) - mScreenWidth ) / 2;
	posY = ( GetSystemMetrics ( SM_CYSCREEN ) - mScreenHeight ) / 2;

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx ( 0, m_applicationName, m_applicationName,
							  WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
							  posX, posY, mScreenWidth, mScreenHeight, NULL, NULL, m_hinstance, NULL );

	// Bring the window up on the screen and set it as main focus.
	ShowWindow ( m_hwnd, SW_SHOW );
	SetForegroundWindow ( m_hwnd );
	SetFocus ( m_hwnd );

	ZeroMemory ( &m_msg, sizeof ( MSG ) );

	Log::Info ( "Windows taks started" );

	return true;
}

void WinPlatform::Update ()
{
	if ( PeekMessage ( &m_msg, NULL, 0, 0, PM_REMOVE ) )
	{
		TranslateMessage ( &m_msg );
		DispatchMessage ( &m_msg );
	}

	if ( m_msg.message == WM_QUIT )
		Platform::SetClosing ();
}

void WinPlatform::Stop ()
{
	// Remove the window.
	DestroyWindow ( m_hwnd );
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass ( m_applicationName, m_hinstance );
	m_hinstance = NULL;

	Log::Info ( "Timer taks stoped" );
}

LRESULT CALLBACK WndProc ( HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam )
{
	switch ( umessage )
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage ( 0 );
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage ( 0 );
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return WinInput::MessageHandler ( hwnd, umessage, wparam, lparam );
		}
	}
}

#endif
