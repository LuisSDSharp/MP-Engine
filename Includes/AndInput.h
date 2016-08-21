#ifndef ANDINPUT_H_
#define ANDINPUT_H_

#include "Input.h"

#if PLATFORM_ANDROID

class AndInput final : public Input
{
public:
	AndInput ( android_app*, const unsigned int );
	virtual ~AndInput ();

	virtual void OnSuspend () override;
	virtual void OnResume () override;
	virtual void Stop () override;

	virtual void RemapKeyCodes () override {}
};

static int32_t android_handle_input ( android_app*, AInputEvent* );

#endif

#endif
