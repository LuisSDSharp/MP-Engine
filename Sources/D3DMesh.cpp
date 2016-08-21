#include "..\Includes\D3DMesh.h"

#if RENDERER_DIRECTX

D3DMesh::D3DMesh ()
{}

D3DMesh::~D3DMesh ()
{
	m_context = 0;
}

void D3DMesh::Initialize ( ID3D11Device* pDevice, ID3D11DeviceContext* pContext )
{
	m_context = pContext;

	HRESULT result;
	D3D11_BUFFER_DESC vBufferDesc, iBufferDesc;
	D3D11_SUBRESOURCE_DATA vData, iData;

	vBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vBufferDesc.ByteWidth = sizeof ( XMFLOAT3 ) * m_elements;
	vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDesc.CPUAccessFlags = 0;
	vBufferDesc.MiscFlags = 0;
	vBufferDesc.StructureByteStride = 0;

	vData.pSysMem = &m_vertices;
	vData.SysMemPitch = 0;
	vData.SysMemSlicePitch = 0;

	result = pDevice->CreateBuffer ( &vBufferDesc, &vData, &m_vBuffer );
	if ( FAILED ( result ) )
	{
		Log::Error ( "D3D could not create mesh's vertex buffer" );
	}

	iBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	iBufferDesc.ByteWidth = sizeof ( USHORT ) * m_elements;
	iBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	iBufferDesc.CPUAccessFlags = 0;
	iBufferDesc.MiscFlags = 0;
	iBufferDesc.StructureByteStride = 0;

	iData.pSysMem = &m_indices;
	iData.SysMemPitch = 0;
	iData.SysMemSlicePitch = 0;

	result = pDevice->CreateBuffer ( &iBufferDesc, &iData, &m_iBuffer );
	if ( FAILED ( result ) )
	{
		Log::Error ( "D3D could not create mesh's index buffer" );
	}
}

void D3DMesh::RegisterMeshData ( const float ( &vertices ) [3] [3], const unsigned short ( &indices ) [3], const unsigned short numElements )
{
	m_elements = numElements;

	for ( USHORT i = 0; i < m_elements; i++ )
	{
		m_vertices.push_back ( XMFLOAT3 ( vertices [i] [0], vertices [i] [1], vertices [i] [2] ) );
		m_indices.push_back ( indices [i] );
	}
}

void D3DMesh::RenderMesh ()
{
	unsigned int stride = sizeof ( XMFLOAT3 );
	unsigned int offset = 0;

	m_context->IASetVertexBuffers ( 0, 1, &m_vBuffer, &stride, &offset );

	m_context->IASetIndexBuffer ( m_iBuffer, DXGI_FORMAT_R32_UINT, 0 );

	m_context->IASetPrimitiveTopology ( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

void D3DMesh::DeallocateBuffers ()
{
	if ( m_iBuffer )
	{
		m_iBuffer->Release ();
		m_iBuffer = 0;
	}

	if ( m_vBuffer )
	{
		m_vBuffer->Release ();
		m_vBuffer = 0;
	}
}

#endif
