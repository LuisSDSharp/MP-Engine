#include "../Includes/AndPlatform.h"
#include "../Includes/AndInput.h"
#include "../Includes/Renderer.h"

#if PLATFORM_ANDROID
AndPlatform::AndPlatform ( android_app* pState, const unsigned int priority ) : Platform ( priority )
{
	m_pState = pState;
}

AndPlatform::~AndPlatform ()
{
	Log::Info ( "AndPlatform taks destroyed" );
}

bool AndPlatform::Start ()
{
	Log::Info ( "AndPlatform taks started" );

	m_pState->onAppCmd = android_handle_cmd;

	return true;
}

void AndPlatform::OnSuspend ()
{
	Log::Info ( "AndPlatform taks suspended" );
}

void AndPlatform::Update ()
{
	int events;
	struct android_poll_source* pSource;

	int ident = ALooper_pollAll ( 0, 0, &events, (void**) &pSource );

	if ( ident >= 0 )
	{
		if ( pSource )
		{
			pSource->process ( m_pState, pSource );
		}

		if ( m_pState->destroyRequested )
			Platform::SetClosing ();
	}
}

void AndPlatform::OnResume ()
{
	Log::Info ( "AndPlatform taks resumed" );
}

void AndPlatform::Stop ()
{
	Log::Info ( "AndPlatform taks stoped" );
}

static void android_handle_cmd ( android_app* app, int cmd )
{
	switch ( cmd )
	{
		case APP_CMD_INIT_WINDOW:
		Log::Info ( "Opening Window" );
		Renderer::SetInitializing ();
		break;

		case APP_CMD_DESTROY:
		Log::Info ( "Destroying app" );
		break;

		case APP_CMD_TERM_WINDOW:
		Log::Info ( "Closing Window" );
		Renderer::SetDestroying ();
		break;

		case APP_CMD_RESUME:
		Log::Info ( "Resuming App" );
		//Platform::SetResuming ();
		break;

		case APP_CMD_PAUSE:
		Log::Info ( "Pausing App" );
		//Platform::SetPausing ();
		break;
	}
}

#endif
