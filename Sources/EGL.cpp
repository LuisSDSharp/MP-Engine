#include "..\Includes\EGL.h"

#if RENDERER_EGL

#include <GLES2\gl2.h>

EGL::EGL ( android_app* pState, const unsigned int priority ) :
	Renderer ( priority ),
	m_display ( EGL_NO_DISPLAY ),
	m_surface ( EGL_NO_SURFACE ),
	m_context ( EGL_NO_CONTEXT ),
	m_width ( 0 ), m_height ( 0 )
{
	m_pState = pState;
}

EGL::~EGL ()
{
	Log::Info ( "EGL taks destroyed" );
}

void EGL::BeginFrame ()
{
	glClearColor ( 0.95f, 0.95f, 0.95f, 1 );
	glClear ( GL_COLOR_BUFFER_BIT );
}

void EGL::EndFrame ()
{
	eglSwapBuffers ( m_display, m_surface );
}

void EGL::Init ()
{
	if ( ( m_display = eglGetDisplay ( EGL_DEFAULT_DISPLAY ) ) == EGL_NO_DISPLAY )
	{
		Log::Error ( "EGL could not get the display" );
	}

	EGLint majorV;
	EGLint minorV;
	if ( !eglInitialize ( m_display, &majorV, &minorV ) )
	{
		Log::Error ( "EGL could not get initialized" );
	}

	Log::Info ( "EGL v%d.%d initialized ", majorV, minorV );

	const EGLint RGBA_8888_ATTRIBS[] =
	{
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_NONE
	};

	const EGLint RGBX_8888_ATTRIBS[] =
	{
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_NONE
	};

	const EGLint RGB_565_ATTRIBS[] =
	{
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RED_SIZE, 5,
		EGL_GREEN_SIZE, 6,
		EGL_BLUE_SIZE, 5,
		EGL_NONE
	};

	const EGLint* attribList;
	EGLint windowFormat = ANativeWindow_getFormat ( m_pState->window );
	switch ( windowFormat )
	{
		case WINDOW_FORMAT_RGBA_8888:
		attribList = RGBA_8888_ATTRIBS;
		break;

		case WINDOW_FORMAT_RGBX_8888:
		attribList = RGBX_8888_ATTRIBS;
		break;

		case WINDOW_FORMAT_RGB_565:
		attribList = RGB_565_ATTRIBS;
		break;

		default:
		Log::Error ( "Unknown window format" );
		break;
	}

	Log::Info ( "Window res %d x %d with format %d",
				ANativeWindow_getWidth ( m_pState->window ),
				ANativeWindow_getHeight ( m_pState->window ),
				windowFormat );

	EGLConfig config;
	EGLint numConfigs;
	if ( !eglChooseConfig ( m_display, attribList, &config, 1, &numConfigs ) )
	{
		Log::Error ( "EGL could not choose configurations" );
	}

	EGLint format;
	if ( !eglGetConfigAttrib ( m_display, config, EGL_NATIVE_VISUAL_ID, &format ) )
	{
		Log::Error ( "EGL could not get configuration attributes" );
	}

	if ( ANativeWindow_setBuffersGeometry ( m_pState->window, 0, 0, format ) != 0 )
	{
		Log::Error ( "EGL could not setup Android's screen geometry" );
	}

	if ( ( m_surface = eglCreateWindowSurface ( m_display, config, m_pState->window, 0 ) ) == EGL_NO_SURFACE )
	{
		Log::Error ( "EGL could not create window surface" );
	}

	if ( ( m_context = eglCreateContext ( m_display, config, 0, 0 ) ) == EGL_NO_CONTEXT )
	{
		Log::Error ( "EGL could not create windo context" );
	}

	if ( !eglMakeCurrent ( m_display, m_surface, m_surface, m_context ) )
	{
		Log::Error ( "EGL could not make the display current" );
	}

	if ( !eglQuerySurface ( m_display, m_surface, EGL_WIDTH, &m_width ) ||
		 !eglQuerySurface ( m_display, m_surface, EGL_HEIGHT, &m_height ) )
	{
		Log::Error ( "EGL could not query the window's surface" );
	}

	Log::Info ( "GPU Vendor: %s, Renderer: %s, Version: %s",
				glGetString ( GL_VENDOR ),
				glGetString ( GL_RENDERER ),
				glGetString ( GL_VERSION ) );

	glClearColor ( 0.5f, 0.5f, 0.5f, 1.0f );

	Log::Info ( "EGL initialized" );

	m_bInitialized = true;
}

void EGL::Destroy ()
{
	if ( m_display != EGL_NO_DISPLAY )
	{
		eglMakeCurrent ( m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );

		if ( m_context != EGL_NO_CONTEXT )
		{
			eglDestroyContext ( m_display, m_context );
			m_context = EGL_NO_CONTEXT;
		}

		if ( m_surface != EGL_NO_SURFACE )
		{
			eglDestroySurface ( m_display, m_surface );
			m_surface = EGL_NO_SURFACE;
		}

		eglTerminate ( m_display );
		m_display = EGL_NO_DISPLAY;
	}

	m_bInitialized = false;

	Log::Info ( "EGL destroyed" );
}

void EGL::OnSuspend ()
{
	Log::Info ( "EGL taks suspended" );
}

void EGL::OnResume ()
{
	Log::Info ( "EGL taks resumed" );
}

bool EGL::Start ()
{
	Log::Info ( "EGL taks started" );
	return true;
}

void EGL::Stop ()
{
	Log::Info ( "EGL taks stopped" );
}

#endif
