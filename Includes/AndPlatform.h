#ifndef ANDROID_H_
#define ANDROID_H_

#include "Platforms.h"
#include "Platform.h"

#if PLATFORM_ANDROID

class AndPlatform final : public Platform
{
private:
	android_app*  m_pState;

public:
	AndPlatform ( android_app*, const unsigned int );
	virtual ~AndPlatform ();

	android_app* GetAppState () { return m_pState; }

	bool  Start ();
	void  OnSuspend ();
	void  Update ();
	void  OnResume ();
	void  Stop ();
};

static void android_handle_cmd ( android_app*, int );

#endif

#endif
