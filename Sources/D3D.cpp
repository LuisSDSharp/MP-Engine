#include "..\Includes\D3D.h"
#include "..\Includes\WinPlatform.h"
#include "..\Includes\Renderable.h"
#include "..\Includes\D3DMesh.h"
#include "..\Includes\HLSL.h"

#if RENDERER_DIRECTX

#include <stdlib.h>

D3D::D3D ( unsigned int priority ) : Renderer ( priority )
{
	m_swapChain				= 0;
	m_device				= 0;
	m_context				= 0;
	m_renderTargetView		= 0;
	m_depthStencilBuffer	= 0;
	m_depthStencilState		= 0;
	m_depthStencilView		= 0;
	m_rasterState			= 0;
}

D3D::~D3D ()
{}

void D3D::BeginFrame ()
{
	float color [4];

	// Setup the color to clear the buffer to.
	color [0] = 1.0f;
	color [1] = 0.0f;
	color [2] = 0.5f;
	color [3] = 1.0f;

	// Clear the back buffer.
	m_context->ClearRenderTargetView ( m_renderTargetView, color );

	// Clear the depth buffer.
	m_context->ClearDepthStencilView ( m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	for each ( Renderable* renderable in m_renderables )
	{
		renderable->GetMesh ()->RenderMesh ();
		renderable->GetShader ()->RenderShader ();
	}
}

void D3D::EndFrame ()
{
	// Present the back buffer to the screen since rendering is complete.
	if ( m_vsync_enabled )
	{
		// Lock to screen refresh rate.
		m_swapChain->Present ( 1, 0 );
	}
	else
	{
		// Present as fast as possible.
		m_swapChain->Present ( 0, 0 );
	}
}

void D3D::RegisterRenderable ( Renderable * pRenderable )
{
	D3DMesh* mesh = dynamic_cast<D3DMesh*>( pRenderable->GetMesh () );
	mesh->Initialize ( m_device, m_context );

	HLSL* shader = dynamic_cast<HLSL*>( pRenderable->GetShader () );
	shader->Initialize ( m_device, &m_projectionMatrix, &m_worldMatrix, &m_orthoMatrix );

	m_renderables.push_back ( pRenderable );
}

void D3D::Init ()
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	size_t stringLength;
	DXGI_MODE_DESC* displayModeList = NULL;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	// Create a DirectX graphics interface factory.
	Log::Assert ( !FAILED ( CreateDXGIFactory ( __uuidof( IDXGIFactory ), (void**) &factory ) ), "Could not create DirectX GI factory" );

	// Use the factory to create an adapter for the primary graphics interface (video card).
	Log::Assert ( !FAILED ( factory->EnumAdapters ( 0, &adapter ) ), "Could not create an adapter for the GPU" );

	// Enumerate the primary adapter output (monitor).
	Log::Assert ( !FAILED ( adapter->EnumOutputs ( 0, &adapterOutput ) ), "Could not enumerate the main monitor" );

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	Log::Assert ( !FAILED ( adapterOutput->GetDisplayModeList ( DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL ) ), "Could not get display mode list" );

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC [numModes];
	Log::Assert ( displayModeList != NULL, "Could not create display/GPU mode list" );

	// Now fill the display mode list structures.
	Log::Assert ( !FAILED ( adapterOutput->GetDisplayModeList ( DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList ) ), "Could not fill display mode list structure" );

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for ( i = 0; i<numModes; i++ )
	{
		if ( displayModeList [i].Width == WINDOW_WIDTH )
		{
			if ( displayModeList [i].Height == WINDOW_HEIGHT )
			{
				numerator = displayModeList [i].RefreshRate.Numerator;
				denominator = displayModeList [i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	Log::Assert ( !FAILED ( adapter->GetDesc ( &adapterDesc ) ), "Could get GPU adapter description" );

	// Store the dedicated video card memory in megabytes.
	m_videoCardMemory = (int) ( adapterDesc.DedicatedVideoMemory / 1024 / 1024 );

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s ( &stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128 );
	Log::Assert ( error == 0, "Could not converte GPU name" );

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release ();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release ();
	adapter = 0;

	// Release the factory.
	factory->Release ();
	factory = 0;

	// Initialize the swap chain description.
	ZeroMemory ( &swapChainDesc, sizeof ( swapChainDesc ) );

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = WINDOW_WIDTH;
	swapChainDesc.BufferDesc.Height = WINDOW_HEIGHT;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if ( m_vsync_enabled )
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = WinPlatform::GetHWND ();

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if ( m_fullscreenEnabled )
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain ( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
											 D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_context );
	Log::Assert ( !FAILED ( result ), "Could not create divice, context and swap chain" );

	// Get the pointer to the back buffer.
	result = m_swapChain->GetBuffer ( 0, __uuidof( ID3D11Texture2D ), (LPVOID*) &backBufferPtr );
	Log::Assert ( !FAILED ( result ), "Could not get pointer to back buffer" );

	// Create the render target view with the back buffer pointer.
	result = m_device->CreateRenderTargetView ( backBufferPtr, NULL, &m_renderTargetView );
	Log::Assert ( !FAILED ( result ), "Could not create target view" );

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release ();
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	ZeroMemory ( &depthBufferDesc, sizeof ( depthBufferDesc ) );

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = WINDOW_WIDTH;
	depthBufferDesc.Height = WINDOW_HEIGHT;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = m_device->CreateTexture2D ( &depthBufferDesc, NULL, &m_depthStencilBuffer );
	Log::Assert ( !FAILED ( result ), "Could not create buffer texture" );

	// Initialize the description of the stencil state.
	ZeroMemory ( &depthStencilDesc, sizeof ( depthStencilDesc ) );

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = m_device->CreateDepthStencilState ( &depthStencilDesc, &m_depthStencilState );
	Log::Assert ( !FAILED ( result ), "Could not create depth stencil state" );

	// Set the depth stencil state.
	m_context->OMSetDepthStencilState ( m_depthStencilState, 1 );

	// Initialize the depth stencil view.
	ZeroMemory ( &depthStencilViewDesc, sizeof ( depthStencilViewDesc ) );

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = m_device->CreateDepthStencilView ( m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView );
	Log::Assert ( !FAILED ( result ), "Could not create depth stencil view" );

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_context->OMSetRenderTargets ( 1, &m_renderTargetView, m_depthStencilView );

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = m_device->CreateRasterizerState ( &rasterDesc, &m_rasterState );
	Log::Assert ( !FAILED ( result ), "Could not create rasterizer state" );

	// Now set the rasterizer state.
	m_context->RSSetState ( m_rasterState );

	// Setup the viewport for rendering.
	viewport.Width = (float) WINDOW_WIDTH;
	viewport.Height = (float) WINDOW_HEIGHT;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_context->RSSetViewports ( 1, &viewport );

	// Setup the projection matrix.
	fieldOfView = 3.141592654f / 4.0f;
	screenAspect = (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT;

	// Create the projection matrix for 3D rendering.
	DirectX::XMStoreFloat4x4 ( &m_projectionMatrix, DirectX::XMMatrixPerspectiveFovLH ( fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH ) );

	// Initialize the world matrix to the identity matrix.
	DirectX::XMStoreFloat4x4 ( &m_worldMatrix, DirectX::XMMatrixIdentity () );

	// Create an orthographic projection matrix for 2D rendering.
	DirectX::XMStoreFloat4x4 ( &m_orthoMatrix, DirectX::XMMatrixOrthographicLH ( (float) WINDOW_WIDTH, (float) WINDOW_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH ) );

	m_bInitialized = true;

	Log::Info ( "D3D taks started" );
}

void D3D::Destroy ()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if ( m_swapChain )
	{
		m_swapChain->SetFullscreenState ( false, NULL );
	}

	if ( m_rasterState )
	{
		m_rasterState->Release ();
		m_rasterState = 0;
	}

	if ( m_depthStencilView )
	{
		m_depthStencilView->Release ();
		m_depthStencilView = 0;
	}

	if ( m_depthStencilState )
	{
		m_depthStencilState->Release ();
		m_depthStencilState = 0;
	}

	if ( m_depthStencilBuffer )
	{
		m_depthStencilBuffer->Release ();
		m_depthStencilBuffer = 0;
	}

	if ( m_renderTargetView )
	{
		m_renderTargetView->Release ();
		m_renderTargetView = 0;
	}

	if ( m_context )
	{
		m_context->Release ();
		m_context = 0;
	}

	if ( m_device )
	{
		m_device->Release ();
		m_device = 0;
	}

	if ( m_swapChain )
	{
		m_swapChain->Release ();
		m_swapChain = 0;
	}

	Log::Info ( "D3D taks stoped" );
}

ID3D11Device* D3D::GetDevice ()
{
	return m_device;
}

ID3D11DeviceContext* D3D::GetDeviceContex ()
{
	return m_context;
}

void D3D::GetProjectionMatrix ( XMFLOAT4X4& projectionMatrix )
{
	projectionMatrix = m_projectionMatrix;
}

void D3D::GetWorldMatrix ( XMFLOAT4X4& worldMatrix )
{
	worldMatrix = m_worldMatrix;
}

void D3D::GetOrthoMatrix ( XMFLOAT4X4& orthoMatix )
{
	orthoMatix = m_orthoMatrix;
}

void D3D::GetVideoCardInfo ( char* cardName, int& memory )
{
	strcpy_s ( cardName, 128, m_videoCardDescription );
	memory = m_videoCardMemory;
}

#endif
