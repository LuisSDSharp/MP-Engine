#ifndef HLSL_H_
#define HLSL_H_

#include "IShader.h"

#if RENDERER_DIRECTX
class D3D;

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

class HLSL : public IShader
{
private:
	struct MatrixBufferType
	{
		XMFLOAT4X4 world;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
	};

	ID3D11DeviceContext*	m_context;
	ID3D11VertexShader*		m_vertexShader;
	ID3D11PixelShader*		m_pixelShader;
	ID3D11InputLayout*		m_layout;
	ID3D11Buffer*			m_matrixBuffer;

	WCHAR*					m_vFilePath;
	WCHAR*					m_pFilePath;
	USHORT					m_indices;

	XMFLOAT4X4*				m_projectionMatrix;
	XMFLOAT4X4*				m_worldMatrix;
	XMFLOAT4X4*				m_orthoMatrix;

public:
	HLSL ();
	virtual ~HLSL ();

	bool					Initialize ( ID3D11Device*, XMFLOAT4X4*, XMFLOAT4X4*, XMFLOAT4X4* );

	// Inherited via IShader
	virtual void			RegisterData ( const char*, const char*, const unsigned short ) override;
	virtual void			DeallocateBuffers () override;
	virtual bool			Setup () override;
	virtual void			RenderShader () override;
};

#endif
#endif
