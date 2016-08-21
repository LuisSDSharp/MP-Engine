#include "..\Includes\RendererComponent.h"
#include "..\Includes\RendererManager.h"
#include "..\Includes\Renderable.h"
#include "..\Includes\GameObject.h"

RendererComponent::RendererComponent ( GameObject* pOwner )
	: Component ( pOwner )
	, m_renderable ( nullptr )
{
	m_manager = RendererManager::GetInstancePtr ();
	m_id = 1;
}

RendererComponent::~RendererComponent ()
{
	m_manager = nullptr;

	if ( m_renderable )
	{
		delete m_renderable;
		m_renderable = nullptr;
	}
}

void RendererComponent::CreateCustomMesh ( float (&vertices)[3][3], const unsigned short (&indices)[3], const unsigned short numElements )
{
	if ( !m_renderable )
	{
		m_renderable = new Renderable ();
		m_renderable->CreateMesh ( vertices, indices, numElements );
	}
}

void RendererComponent::CreateShader ( const char* vFilePath, const char* fFilePath )
{
	if ( m_renderable )
	{
		m_renderable->CreateShader ( vFilePath, fFilePath );
	}
	else
	{
		Log::Error ( "A Mesh needs to be created first before a Shader" );
	}
}

void RendererComponent::Initialize ()
{
	if ( m_renderable )
	{
		m_manager->RegisterRenderable ( m_renderable );
	}
	else
	{
		Log::Error ( "A Mesh needs to be created before initializing a Render Component as least" );
	}
}
