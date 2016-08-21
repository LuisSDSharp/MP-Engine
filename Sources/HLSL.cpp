#include "..\Includes\HLSL.h"
#include "..\Includes\Renderer.h"
#include "..\Includes\D3D.h"

#if RENDERER_DIRECTX

HLSL::HLSL ()
	: m_vertexShader ( 0 )
	, m_pixelShader ( 0 )
	, m_layout ( 0 )
	, m_matrixBuffer ( 0 )
{}

HLSL::~HLSL ()
{
	m_context			= 0;
	m_vFilePath			= 0;
	m_pFilePath			= 0;
	m_projectionMatrix	= 0;
	m_worldMatrix		= 0;
	m_orthoMatrix		= 0;
}

bool HLSL::Initialize ( ID3D11Device* pDevice, XMFLOAT4X4* pProjection, XMFLOAT4X4* pWorld, XMFLOAT4X4* pView )
{
	m_projectionMatrix = pProjection;
	m_worldMatrix = pWorld;
	m_orthoMatrix = pView;

	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout [2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	result = D3DCompileFromFile ( m_vFilePath, NULL, NULL, "VertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage );
	if ( FAILED ( result ) )
	{
		if ( errorMessage )
		{
			char* e = (char*) ( errorMessage->GetBufferPointer () );
			Log::Error ( "Error compiling vertex shader: %s", e );
		}
		else
		{
			Log::Error ( "Error finding %s shader file", m_vFilePath );
		}

		return false;
	}

	result = D3DCompileFromFile ( m_pFilePath, NULL, NULL, "PixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage );
	if ( FAILED ( result ) )
	{
		if ( errorMessage )
		{
			char* e = (char*) ( errorMessage->GetBufferPointer () );
			Log::Error ( "Error compiling pixel shader: %s", e );
		}
		else
		{
			Log::Error ( "Error finding %s shader file", m_pFilePath );
		}

		return false;
	}

	result = pDevice->CreateVertexShader ( vertexShaderBuffer->GetBufferPointer (), vertexShaderBuffer->GetBufferSize (), NULL, &m_vertexShader );
	if ( FAILED ( result ) )
		return false;

	result = pDevice->CreatePixelShader ( pixelShaderBuffer->GetBufferPointer (), pixelShaderBuffer->GetBufferSize (), NULL, &m_pixelShader );
	if ( FAILED ( result ) )
		return false;

	polygonLayout [0].SemanticName = "POSITION";
	polygonLayout [0].SemanticIndex = 0;
	polygonLayout [0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout [0].InputSlot = 0;
	polygonLayout [0].AlignedByteOffset = 0;
	polygonLayout [0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout [0].InstanceDataStepRate = 0;

	polygonLayout [1].SemanticName = "COLOR";
	polygonLayout [1].SemanticIndex = 0;
	polygonLayout [1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout [1].InputSlot = 0;
	polygonLayout [1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout [1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout [1].InstanceDataStepRate = 0;

	numElements = sizeof ( polygonLayout ) / sizeof ( polygonLayout [0] );

	result = pDevice->CreateInputLayout ( polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer (), vertexShaderBuffer->GetBufferSize (), &m_layout );
	if ( FAILED ( result ) )
		return false;

	vertexShaderBuffer->Release ();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release ();
	pixelShaderBuffer = 0;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof ( MatrixBufferType );
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = pDevice->CreateBuffer ( &matrixBufferDesc, NULL, &m_matrixBuffer );
	if ( FAILED ( result ) )
		return false;

	return true;
}

void HLSL::RegisterData ( const char* vFilePath, const char* pFilePath, const unsigned short numIndices )
{
	m_vFilePath = (WCHAR*) vFilePath;
	m_pFilePath = (WCHAR*) pFilePath;
	m_indices = numIndices;
}

void HLSL::DeallocateBuffers ()
{
	if ( m_matrixBuffer )
	{
		m_matrixBuffer->Release ();
		m_matrixBuffer = 0;
	}

	if ( m_layout )
	{
		m_layout->Release ();
		m_layout = 0;
	}

	if ( m_pixelShader )
	{
		m_pixelShader->Release ();
		m_pixelShader = 0;
	}

	if ( m_vertexShader )
	{
		m_vertexShader->Release ();
		m_vertexShader = 0;
	}
}

bool HLSL::Setup ()
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	XMMATRIX projectionM = XMLoadFloat4x4 ( m_projectionMatrix );
	XMStoreFloat4x4 ( m_projectionMatrix, XMMatrixTranspose ( projectionM ) );

	XMMATRIX worldM = XMLoadFloat4x4 ( m_worldMatrix );
	XMStoreFloat4x4 ( m_worldMatrix, XMMatrixTranspose ( worldM ) );

	XMMATRIX orthoM = XMLoadFloat4x4 ( m_orthoMatrix );
	XMStoreFloat4x4 ( m_orthoMatrix, XMMatrixTranspose ( orthoM ) );

	result = m_context->Map ( m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	if ( FAILED ( result ) )
	{
		return false;
	}

	dataPtr = (MatrixBufferType*) mappedResource.pData;
	dataPtr->projection = *m_projectionMatrix;
	dataPtr->world = *m_worldMatrix;
	dataPtr->view = *m_orthoMatrix;

	m_context->Unmap ( m_matrixBuffer, 0 );

	bufferNumber = 0;

	m_context->VSSetConstantBuffers ( bufferNumber, 1, &m_matrixBuffer );

	return true;
}

void HLSL::RenderShader ()
{
	if ( Setup () )
	{
		m_context->IASetInputLayout ( m_layout );
		m_context->VSSetShader ( m_vertexShader, NULL, 0 );
		m_context->PSSetShader ( m_pixelShader, NULL, 0 );

		m_context->DrawIndexed ( m_indices, 0, 0 );
	}
}

#endif