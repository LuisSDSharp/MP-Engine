#include "..\Includes\AndInput.h"

#if PLATFORM_ANDROID

AndInput::AndInput ( android_app* pState, unsigned int priority ) : Input ( priority )
{
	pState->onInputEvent = android_handle_input;
}

AndInput::~AndInput ()
{
	Log::Info ( "AndInput taks destroyed" );
}

void AndInput::OnSuspend ()
{
	Log::Info ( "AndInput taks suspended" );
}

void AndInput::OnResume ()
{
	Log::Info ( "AndInput taks resumed" );
}

void AndInput::Stop ()
{
	Log::Info ( "AndInput taks stoped" );
}

static int32_t android_handle_input ( android_app* app, AInputEvent* event )
{
	switch ( AInputEvent_getType ( event ) )
	{
		case AINPUT_EVENT_TYPE_MOTION:
		return 1;

		case AINPUT_SOURCE_TOUCHSCREEN:
		Log::Info ( "TOUCH" );
		return 1;

		default:
		return 0;
	}
}

#endif
