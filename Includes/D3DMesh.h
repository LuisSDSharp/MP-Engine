#ifndef D3DMESH_H_
#define D3DMESH_H_

#include "IMesh.h"

#if RENDERER_DIRECTX
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

class D3DMesh : public IMesh
{
private:
	ID3D11Buffer*				m_vBuffer;
	ID3D11Buffer*				m_iBuffer;
	ID3D11DeviceContext*		m_context;
	std::vector<XMFLOAT3>		m_vertices;
	std::vector<USHORT>			m_indices;
	USHORT						m_elements;

public:
	D3DMesh ();
	virtual ~D3DMesh ();

	void						Initialize ( ID3D11Device*, ID3D11DeviceContext* );

	// Inherited via IMesh
	virtual void				RegisterMeshData ( const float (&) [3] [3], const unsigned short (&) [3], const unsigned short ) override;
	virtual void				RenderMesh () override;
	virtual void				DeallocateBuffers () override;
	virtual unsigned short		GetNumElements () override { return m_elements; }
};

#endif
#endif
