#include "..\Includes\Application.h"

#if PLATFORM_ANDROID
	#include "..\Includes\AndPlatform.h"
	#include "..\Includes\AndInput.h"
#elif PLATFORM_WINDOWS
	#include "..\Includes\WinPlatform.h"
	#include "..\Includes\WinInput.h"
#elif PLATFORM_IOS
#endif

#if RENDERER_OPENGL
	#include "..\Includes\OpenGL.h"
#elif RENDERER_DIRECTX
	#include "..\Includes\D3D.h"
#elif RENDERER_EGL
	#include "..\Includes\EGL.h"
#elif RENDERER_VULKAN
#endif

#include "..\Includes\Canvas.h"

Application::Application () : m_timer ( ITask::TIMER_PRIORITY )
{
	#if PLATFORM_WINDOWS
	m_platform = new WinPlatform ( ITask::PLATFORM_PRIORITY );
	m_input = new WinInput ( ITask::INPUT_PRIORITY );

	#if RENDERER_OPENGL
	m_renderer = new OpenGL ( ITask::RENDERER_PRIORITY );
	#elif RENDERER_DIRECTX
	m_renderer = new D3D ( ITask::RENDERER_PRIORITY );
	#endif
	#endif
}

#if PLATFORM_ANDROID
Application::Application ( android_app* pState ) : m_timer ( ITask::TIMER_PRIORITY )
{
	m_platform = new AndPlatform ( pState, ITask::PLATFORM_PRIORITY );
	m_input = new AndInput ( pState, ITask::INPUT_PRIORITY );
	m_renderer = new EGL ( pState, ITask::RENDERER_PRIORITY );
}
#endif

Application::~Application ()
{
	Log::Info ( "Application desctructor" );

	delete m_platform;
	//Log::Assert ( !m_platform, "Platform taks memory leaked" );
	m_platform = nullptr;

	delete m_input;
	//Log::Assert ( m_input == nullptr, "Input taks memory leaked" );
	m_input = nullptr;

	delete m_renderer;
	//Log::Assert ( m_renderer == nullptr, "Renderer taks memory leaked" );
	m_renderer = nullptr;

	delete m_canvas;
	//Log::Assert ( m_canvas == nullptr, "Canvas taks memory leaked" );
	m_canvas = nullptr;
}

bool Application::Initialize ()
{
	return ( m_kernel.AddTask ( &m_timer ) &&
			 m_kernel.AddTask ( m_platform  ) &&
			 m_kernel.AddTask ( m_input ) &&
			 m_kernel.AddTask ( m_renderer ) );
}

void Application::Run ()
{
	m_kernel.Execute ();
}

bool Application::AddCanvasKernelTask ( Canvas* pTask )
{
	m_canvas = pTask;
	return m_kernel.AddTask ( m_canvas );
}

bool Application::AddCustomKernelTask ( ITask* pTask )
{
	return m_kernel.AddTask ( pTask );
}
