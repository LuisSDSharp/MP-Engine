#ifndef WINDOWS_H_
#define WINDOWS_H_

#include "Platform.h"

#if PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#define NOCOMM
#include <Windows.h>

class WinPlatform final : public Platform
{
public:
	WinPlatform ( const unsigned int priority );
	virtual ~WinPlatform ();

	virtual bool				Start ();
	virtual void				Update ();
	virtual void				Stop ();

	static HWND					GetHWND () { return m_hwnd; }

private:
	MSG							m_msg;
	LPCWSTR						m_applicationName;
	HINSTANCE					m_hinstance;
	static HWND					m_hwnd;

	unsigned int				mScreenWidth = 800;
	unsigned int				mScreenHeight = 600;
};

static LRESULT CALLBACK WndProc ( HWND, UINT, WPARAM, LPARAM );

#endif

#endif