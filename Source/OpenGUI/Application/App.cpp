#include "App.h"
#include <d3d11.h>
#include <Windowsx.h>
#include <d3dcompiler.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include "fcntl.h"

#include <imgui.h>

#include "Editor.h"

struct Vertex
{
	float Pos[3];
	float TexCoord[2];
};

MyApp::MyApp( HINSTANCE hInstance )
	:BaseApp( hInstance )
	, ScreenVertexBuffer( nullptr )
	, ScreenIndexBuffer( nullptr )
	, mSampleLinear( nullptr )
	, mRTResource( nullptr )
	, mRTSRV( nullptr )
	, mLessEqual( nullptr )
	, mNoCullRS( nullptr )
	, mpVertexShader( nullptr )
	, mpPixelShader( nullptr )
	, mInputLayout( nullptr )
{
	srand( ( unsigned char )( time( NULL ) ) );
}

MyApp::~MyApp()
{
	ReleaseCOM( ScreenVertexBuffer );
	ReleaseCOM( ScreenIndexBuffer );

	ReleaseCOM( mSampleLinear );
}

bool MyApp::Init()
{
	if( !BaseApp::Init() )	return false;

	CreatelScreenQuadBuffer();

	if( !InitRenderState() )	return false;

	if( !InitShader() )	return false;

	ResizeRT( mClientWidth , mClientHeight );

	SetupImStyle( true , 0.5f );

	return true;
}

void MyApp::OnResize()
{
	assert( md3dImmediateContext );
	assert( md3dDevice );
	assert( mSwapChain );

	ReleaseCOM( mRenderTargetView );
	ReleaseCOM( mDepthStencilView );
	ReleaseCOM( mDepthStencilBuffer );

	HR( mSwapChain->ResizeBuffers( 1 , mClientWidth , mClientHeight , DXGI_FORMAT_R8G8B8A8_UNORM , 0 ) );
	ID3D11Texture2D* backBuffer;
	HR( mSwapChain->GetBuffer( 0 , __uuidof( ID3D11Texture2D ) , reinterpret_cast< void** >( &backBuffer ) ) );
	HR( md3dDevice->CreateRenderTargetView( backBuffer , 0 , &mRenderTargetView ) );
	ReleaseCOM( backBuffer );

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = mClientWidth;
	depthStencilDesc.Height = mClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if( mEnable4xMsaa )
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}

	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags      = 0;

	HR( md3dDevice->CreateTexture2D( &depthStencilDesc , 0 , &mDepthStencilBuffer ) );
	HR( md3dDevice->CreateDepthStencilView( mDepthStencilBuffer , 0 , &mDepthStencilView ) );

	md3dImmediateContext->OMSetRenderTargets( 1 , &mRenderTargetView , mDepthStencilView );

	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width    = static_cast< float >( mClientWidth );
	mScreenViewport.Height   = static_cast< float >( mClientHeight );
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	md3dImmediateContext->RSSetViewports( 1 , &mScreenViewport );
}

void MyApp::Update( float dt )
{
	D3D11_MAPPED_SUBRESOURCE MapData;
	if( FAILED( md3dImmediateContext->Map( mRTResource , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &MapData ) ) )
	{
		return;
	}

	float* pData = reinterpret_cast< float* >( MapData.pData );

	int RowPitch = MapData.RowPitch;

	D3D11_TEXTURE2D_DESC desc;
	mRTResource->GetDesc( &desc );

	for( int irow = 0; irow < desc.Height; irow++ )
	{
		for( int icol = 0; icol < desc.Width; icol++ )
		{
			pData[icol * 4 + 0] = 1.0f;
			pData[icol * 4 + 1] = 1.0f;
			pData[icol * 4 + 2] = ( float )rand() / RAND_MAX;
			pData[icol * 4 + 3] = 0.5f;
		}

		pData += RowPitch / sizeof( float );
	}
	md3dImmediateContext->Unmap( mRTResource , 0 );
}

void MyApp::Render()
{
	Editor();

	float Yellow[4] = { 0.75 , 0.75 , 0.75 , 1.0 };
	md3dImmediateContext->ClearRenderTargetView( mRenderTargetView , reinterpret_cast< const float* >( Yellow ) );
	md3dImmediateContext->ClearDepthStencilView( mDepthStencilView , D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL , 1.0f , 0 );

	md3dImmediateContext->RSSetState( mNoCullRS );

	md3dImmediateContext->OMSetDepthStencilState( mLessEqual , 0 );

	RenderToQuad( &mRTSRV );

	ImGui::Render();

	HR( mSwapChain->Present( 0 , 0 ) );
}

void MyApp::RenderToQuad( ID3D11ShaderResourceView* const * srv )
{
	UINT stride = sizeof( Vertex );
	UINT offset = 0;

	md3dImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	md3dImmediateContext->IASetVertexBuffers( 0 , 1 , &ScreenVertexBuffer , &stride , &offset );
	md3dImmediateContext->IASetIndexBuffer( ScreenIndexBuffer , DXGI_FORMAT_R32_UINT , 0 );

	md3dImmediateContext->IASetInputLayout( mInputLayout );

	md3dImmediateContext->VSSetShader( mpVertexShader , nullptr , 0 );
	md3dImmediateContext->PSSetShader( mpPixelShader , nullptr , 0 );

	md3dImmediateContext->PSSetShaderResources( 0 , 1 , srv );
	md3dImmediateContext->PSSetSamplers( 0 , 1 , &mSampleLinear );

	md3dImmediateContext->DrawIndexed( 6 , 0 , 0 );
}

bool MyApp::InitRenderState()
{
	D3D11_DEPTH_STENCIL_DESC LessEqualDepthStencilDesc;
	LessEqualDepthStencilDesc.DepthEnable = true;
	LessEqualDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	LessEqualDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	LessEqualDepthStencilDesc.StencilEnable = false;

	if( FAILED( md3dDevice->CreateDepthStencilState( &LessEqualDepthStencilDesc , &mLessEqual ) ) )
	{
		return false;
	}

	D3D11_RASTERIZER_DESC NoCullDesc;
	ZeroMemory( &NoCullDesc , sizeof( D3D11_RASTERIZER_DESC ) );
	NoCullDesc.FillMode = D3D11_FILL_SOLID;					//实体填充
	NoCullDesc.CullMode = D3D11_CULL_NONE;					//不剔除
	NoCullDesc.FrontCounterClockwise = false;				//顺时针为正面
	NoCullDesc.DepthClipEnable = true;						//启用深度剪裁

	if( FAILED( md3dDevice->CreateRasterizerState( &NoCullDesc , &mNoCullRS ) ) )
	{
		return false;
	}

	D3D11_SAMPLER_DESC SampleDesc;
	ZeroMemory( &SampleDesc , sizeof( SampleDesc ) );

	SampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	SampleDesc.MipLODBias = 0.0f;
	SampleDesc.MaxAnisotropy = 1;

	SampleDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;

	SampleDesc.BorderColor[0] = 0;
	SampleDesc.BorderColor[1] = 0;
	SampleDesc.BorderColor[2] = 0;
	SampleDesc.BorderColor[3] = 0;

	SampleDesc.MinLOD = 0;
	SampleDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HR( md3dDevice->CreateSamplerState( &SampleDesc , &mSampleLinear ) );

	return true;
}

void MyApp::CreatelScreenQuadBuffer()
{
	Vertex vertices[] =
	{
		{ -1.0f,   -1.0f, 0.5f ,	0.0f, 1.0f } ,
		{ -1.0f,	1.0f, 0.5f ,	0.0f, 0.0f } ,
		{ 1.0f ,	1.0f, 0.5f ,	1.0f, 0.0f } ,
		{ 1.0f ,   -1.0f, 0.5f ,	1.0f, 1.0f }
	};

	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	VertexBufferDesc.ByteWidth = sizeof( Vertex ) * 4;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA VertexInitData;
	VertexInitData.pSysMem = &vertices[0];

	HR( md3dDevice->CreateBuffer( &VertexBufferDesc , &VertexInitData , &ScreenVertexBuffer ) );

	UINT indices[6] =
	{
		0 , 1 , 2 ,
		0 , 2 , 3
	};

	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	IndexBufferDesc.ByteWidth = sizeof( UINT ) * 6;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA IndexInitData;
	IndexInitData.pSysMem = indices;

	HR( md3dDevice->CreateBuffer( &IndexBufferDesc , &IndexInitData , &ScreenIndexBuffer ) );
}

bool MyApp::InitShader()
{
	ID3DBlob* pVertexBlob;
	D3DCompileFromFile( L"Shader/DebugTextureVertexShader.hlsl" , nullptr , nullptr , "main" , "vs_4_0" , 0 , 0 , &pVertexBlob , nullptr );

	if( FAILED( md3dDevice->CreateVertexShader( pVertexBlob->GetBufferPointer() , pVertexBlob->GetBufferSize() , nullptr , &mpVertexShader ) ) )
	{
		return false;
	}

	ID3DBlob* pPixelBlob;
	D3DCompileFromFile( L"Shader/DebugTexturePixelShader.hlsl" , nullptr , nullptr , "main" , "ps_4_0" , 0 , 0 , &pPixelBlob , nullptr );
	if( FAILED( md3dDevice->CreatePixelShader( pPixelBlob->GetBufferPointer() , pPixelBlob->GetBufferSize() , nullptr , &mpPixelShader ) ) )
	{
		return false;
	}


	D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
	{
		{ "POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0  , 0 , D3D11_INPUT_PER_VERTEX_DATA , 0 } ,
		{ "TEXCOORD" , 0 , DXGI_FORMAT_R32G32_FLOAT , 0 , 12 , D3D11_INPUT_PER_VERTEX_DATA , 0 }
	};

	UINT size = ARRAYSIZE( InputLayoutDesc );

	if( FAILED( md3dDevice->CreateInputLayout( InputLayoutDesc , size , pVertexBlob->GetBufferPointer() , pVertexBlob->GetBufferSize() , &mInputLayout ) ) )
	{
		return false;
	}

	return true;
}

void MyApp::ResizeRT( int width , int height )
{
	ReleaseCOM( mRTSRV );
	ReleaseCOM( mRTResource );

	mRTResource = nullptr;

	D3D11_TEXTURE2D_DESC tex_desc;
	ZeroMemory( &tex_desc , sizeof( tex_desc ) );
	tex_desc.Width = width;
	tex_desc.Height = height;
	tex_desc.MipLevels = 1;
	tex_desc.ArraySize = 1;
	tex_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.Usage = D3D11_USAGE_DYNAMIC;				// can be modify
	tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tex_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tex_desc.MiscFlags = 0;

	// Init Texture Data
	float* pData = new float[width * height * 4];
	for( int irow = 0; irow < height; irow++ )
	{
		for( int icol = 0; icol < width; icol++ )
		{
			for( int k = 0; k < 4; k++ )
			{
				pData[irow * 4 * width + icol * 4 + k] = ( 255 / ( k + 1 ) ) / 255.0f;
			}
		}
	}
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData , sizeof( D3D11_SUBRESOURCE_DATA ) );
	InitData.pSysMem = pData;
	InitData.SysMemPitch = sizeof( float ) * width * 4;

	HR( md3dDevice->CreateTexture2D( &tex_desc , &InitData, &mRTResource ) );

	//Create the resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	ZeroMemory( &srv_desc , sizeof( srv_desc ) );
	srv_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = 1;
	srv_desc.Texture2D.MostDetailedMip = 0;

	HR( md3dDevice->CreateShaderResourceView( mRTResource , &srv_desc , &mRTSRV ) );
}