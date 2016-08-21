#ifndef D3D_H_
#define D3D_H_

#include "Renderer.h"

#if RENDERER_DIRECTX

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class D3D final : public Renderer
{
public:
	D3D (unsigned int priority);
	virtual ~D3D ();

	virtual void			Init () override;
	virtual void			Destroy () override;

	ID3D11Device*			GetDevice ();
	ID3D11DeviceContext*	GetDeviceContex ();

	void					GetProjectionMatrix ( XMFLOAT4X4& );
	void					GetWorldMatrix ( XMFLOAT4X4& );
	void					GetOrthoMatrix ( XMFLOAT4X4& );

	void					GetVideoCardInfo ( char*, int& );

private:
	virtual void			BeginFrame () override;
	virtual void			EndFrame () override;

	bool					m_vsync_enabled;
	int						m_videoCardMemory;
	char					m_videoCardDescription [128];
	IDXGISwapChain*			m_swapChain;
	ID3D11Device*			m_device;
	ID3D11DeviceContext*	m_context;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D*		m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState*	m_rasterState;
	XMFLOAT4X4				m_projectionMatrix;
	XMFLOAT4X4				m_worldMatrix;
	XMFLOAT4X4				m_orthoMatrix;

	// Inherited via Renderer
	virtual bool Start () override { Init (); return true; }
	virtual void Stop () override { Destroy (); }

	// Inherited via Renderer
	virtual void RegisterRenderable ( Renderable* pRenderable ) override;
};
#endif

#endif