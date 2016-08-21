#ifndef WININPUT_H_
#define WININPUT_H_

#include "Input.h"

#if PLATFORM_WINDOWS

#include <Windows.h>

class WinInput final : public Input
{
private:
	static UINT	  m_msg;
	static WPARAM m_wparam;

public:
	WinInput ( const unsigned int );
	virtual ~WinInput ();

	virtual void				Update () override;
	virtual void				Stop () override { Log::Info ( "Win Input task stoped" ); }

	virtual void				RemapKeyCodes () override;

	static LRESULT CALLBACK		MessageHandler ( HWND, UINT, WPARAM, LPARAM );
};

#endif

#endif