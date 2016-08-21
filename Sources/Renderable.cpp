#include "..\Includes\Renderable.h"

#if RENDERER_DIRECTX
#include "..\Includes\D3DMesh.h"
#include "..\Includes\HLSL.h"
#endif

Renderable::Renderable ()
{}


Renderable::~Renderable ()
{
	m_mesh->DeallocateBuffers ();
	delete m_mesh;
	m_mesh = nullptr;

	m_shader->DeallocateBuffers ();
	delete m_shader;
	m_shader = nullptr;
}

void Renderable::CreateMesh ( const float ( &vertices ) [3] [3], const unsigned short ( &indices ) [3], const unsigned short numElements )
{
	#if RENDERER_DIRECTX
	m_mesh = new D3DMesh ();
	#endif

	m_mesh->RegisterMeshData ( vertices, indices, numElements );
}

void Renderable::CreateShader ( const char* vFilePath, const char* fFilePath )
{
	#if RENDERER_DIRECTX
	m_shader = new HLSL ();
	#endif

	m_shader->RegisterData ( vFilePath, fFilePath, m_mesh->GetNumElements () );
}
