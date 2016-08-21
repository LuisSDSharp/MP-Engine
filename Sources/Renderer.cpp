#include "../Includes/Renderer.h"
#include "../Includes/RendererManager.h"

bool Renderer::g_initializing = false;
bool Renderer::g_destroying = false;

Renderer::Renderer ( unsigned int priority ) :
	ITask ( priority ), m_fullscreenEnabled ( false ),
	m_vsyncEnabled ( false ), m_bInitialized ( false )
{
	m_manager = new RendererManager ( this );
}

Renderer::~Renderer ()
{
	delete m_manager;
	m_manager = nullptr;
}

void Renderer::Update ()
{
	if ( m_bInitialized )
	{
		RenderFrame ();
	}

	if ( g_initializing )
	{
		Init ();
		g_initializing = false;
	}

	if ( g_destroying )
	{
		Destroy ();
		g_destroying = false;
	}
}

void Renderer::RenderFrame ()
{
	BeginFrame ();
	EndFrame ();
}
