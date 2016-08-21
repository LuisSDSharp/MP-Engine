#include "..\Includes\RendererManager.h"
#include "..\Includes\Renderer.h"

#ifndef NDEBUG
	#include "..\Includes\Logger.h"
#endif

RendererManager::RendererManager ( Renderer* const pRenderer ) : ISingleton (), m_owner ( pRenderer )
{}

RendererManager::~RendererManager ()
{
	m_owner = nullptr;
}

void RendererManager::RegisterRenderable ( Renderable* pRenderable )
{
	m_owner->RegisterRenderable ( pRenderable );
}
