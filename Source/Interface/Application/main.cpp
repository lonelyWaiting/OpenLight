#include <Common/d3dApp.h>
#include <Common/GeometryGenerator.h>
#include <Common/MathHelper.h>
#include <Common/RenderState.h>
#include <Common/TextureMgr.h>
#include <EffectFactory/Vertex.h>

#include <EffectFactory/DebugTextureEffectFactory.h>

#include <imgui.h>
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>

#include <Application/OpenLight.h>

struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 Tex;
};

class MeshViewApp : public D3DApp
{
public:
	MeshViewApp( HINSTANCE hInstance );
	~MeshViewApp();

public:
	bool Init();
	void OnResize();
	void UpdateScene( float dt );
	void DrawScene();

	void OnMouseDown( WPARAM buttonState , int x , int y );
	void OnMouseUp( WPARAM buttonState , int x , int y );
	void OnMouseMove( WPARAM buttonState , int x , int y );

private:
	void DrawScreenQuad( ID3D11ShaderResourceView* const * srv );
	void BuildScreenQuadGeometryBuffers();

	void BuildResource();
	bool BuildEffectFactory();

public:
	void ResizeTexture( int width , int height );

private:
	TextureMgr mTexMgr;

	ID3D11Buffer* mScreenQuadVertexBuffer;
	ID3D11Buffer* mScreenQuadIndexBuffer;

	ID3D11SamplerState* mSampleLinear;

	// Effect Factory
	DebugTextureEffectFactory* mDebugTextureEffectFactory;

	ID3D11ShaderResourceView* mTextureSRV;
	ID3D11Texture2D* mTexture;

	POINT mLastMousePos;

	Scene* scene;

	Camera* camera;

	SurfaceIntegrator* pSurfaceIntegrator;

	Sampler* pSampler;

	Renderer* renderer;

public:
	// UI data
	int		Width;
	int		Height;
	float	fov;
	int		MaxDepth;
	int		spp;

public:
	bool bResize;
};


int WINAPI WinMain( HINSTANCE hInstance , HINSTANCE prevInstance , PSTR cmdLine , int showCmd )
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	MeshViewApp theApp( hInstance );

	if( !theApp.Init() )
		return 0;

	return theApp.Run();
}


MeshViewApp::MeshViewApp( HINSTANCE hInstance )
	:D3DApp( hInstance ) ,

	// Screen Quad Vertex / Index Buffer
	mScreenQuadVertexBuffer( nullptr ) ,
	mScreenQuadIndexBuffer( nullptr ) ,

	mSampleLinear( nullptr ) ,

	// Effect Factory
	mDebugTextureEffectFactory( nullptr ) ,

	mTexture( nullptr ) ,
	mTextureSRV( nullptr ) ,
	Width( 0 ) ,
	Height( 0 ) ,
	fov( 0.0f ) ,
	spp( 0 ) ,
	MaxDepth( 0 ) ,
	bResize( false )
{
	mMainWndCaption = TEXT( "MeshView Demo" );

	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	scene = nullptr;

	camera = nullptr;

	pSurfaceIntegrator = nullptr;

	pSampler = nullptr;

	renderer = nullptr;
}


MeshViewApp::~MeshViewApp()
{
	RenderStates::DestroyAll();

	ReleaseCOM( mScreenQuadVertexBuffer );
	ReleaseCOM( mScreenQuadIndexBuffer );

	SafeDelete( mDebugTextureEffectFactory );

	ReleaseCOM( mSampleLinear );

	SafeDelete( scene );

	SafeDelete( camera );

	SafeDelete( pSurfaceIntegrator );

	SafeDelete( pSampler );

	SafeDelete( renderer );
}


bool MeshViewApp::Init()
{
	if( !D3DApp::Init() )
	{
		return false;
	}

	RenderStates::InitAll( md3dDevice );

	mTexMgr.Init( md3dDevice );

	// Create Shader Effect
	bool result = BuildEffectFactory();
	if( !result )	return false;

	// Create Sampler and Shader Resource View , pass to programable pipeline
	BuildScreenQuadGeometryBuffers();

	ShowWindow( mhMainWnd , SW_SHOWDEFAULT );
	UpdateWindow( mhMainWnd );

	ImGui_ImplDX11_Init( mhMainWnd , md3dDevice , md3dImmediateContext );

	InitRTTI();

	srand( ( unsigned int )time( NULL ) );

	scene = new Scene;

	camera = nullptr;

	pSurfaceIntegrator = nullptr;

	pSampler = nullptr;

	renderer = DeserializationScene( scene , camera , pSurfaceIntegrator , pSampler );

	Width = camera->GetFilm()->GetResolution().x;
	Height = camera->GetFilm()->GetResolution().y;
	fov = camera->GetFovy();
	spp = renderer->GetSpp();
	MaxDepth = pSurfaceIntegrator->GetMaxRecusiveDepth();

	BuildResource();

	return true;
}


void MeshViewApp::OnResize()
{
	D3DApp::OnResize();
}


void MeshViewApp::UpdateScene( float dt )
{

}


void MeshViewApp::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView( mRenderTargetView , reinterpret_cast< const float* >( &Colors::Yellow ) );
	md3dImmediateContext->ClearDepthStencilView( mDepthStencilView , D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL , 1.0f , 0 );

	md3dImmediateContext->RSSetState( RenderStates::mNoCullRS );

	md3dImmediateContext->OMSetDepthStencilState( RenderStates::mLessEqual , 0 );


	ImGui_ImplDX11_NewFrame();

	int bBeginRendering = 0;
	int bSave = 0;

	{
		ImGui::Text( "Hello, world!" );
		ImGui::SliderFloat( "fov" , &fov , 45.0f , 180.0f );
		ImGui::SliderInt( "MaxDepth" , &MaxDepth , 1 , 20 );
		ImGui::InputInt( "spp" , &spp , 8 );
		if( ImGui::InputInt( "Image Width" , &Width , 10 ) )
		{
			bResize = true;
		}
		if( ImGui::InputInt( "Image Height" , &Height , 10 ) )
		{
			bResize = true;
		}

		if ( ImGui::Button( "Begin Rending" ) ) bBeginRendering ^= 1;
		if( ImGui::Button( "Save Scene" ) )	bSave ^= 1;
	}

	if( bBeginRendering )
	{
		if( bResize )
		{
			ResizeTexture( Width , Height );
			camera->GetFilm()->Resize( Width , Height );
			bResize = false;
		}

		if( renderer != nullptr )
		{
			camera->SetFovy( fov );
			pSurfaceIntegrator->SetMaxRecusiveDepth( MaxDepth );
			renderer->SetSpp( spp );

			renderer->Render( scene );

			// Update Window Image
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory( &mappedResource , sizeof( D3D11_MAPPED_SUBRESOURCE ) );
			md3dImmediateContext->Map( mTexture , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &mappedResource );

			DirectX::XMFLOAT4* data = static_cast< DirectX::XMFLOAT4* >( mappedResource.pData );

			Image* image = camera->GetFilm()->GetImage();
			for( int col = 0; col < Width; col++ )
			{
				for( int row = 0; row < Height; row++ )
				{	
					Spectrum sourcedata = image->GetColor( row , col );
					double rgb[3];
					sourcedata.ToRGB( rgb );

					data[row * Width + col].x = /*( float )rgb[0]*/( ( row & 1 ) || ( col & 1 ) ) ? 0.0f : 1.0f;
					data[row * Width + col].y = /*( float )rgb[1]*/( ( row & 1 ) || ( col & 1 ) ) ? 0.0f : 1.0f;
					data[row * Width + col].z = /*( float )rgb[2]*/( ( row & 1 ) || ( col & 1 ) ) ? 0.0f : 1.0f;
					data[row * Width + col].w = 1.0f;
				}
			}
			//	Reenable GPU access to the vertex buffer data.
			md3dImmediateContext->Unmap( mTexture , 0 );
		}
	}

	if( bSave )
	{
		SerializationScene( scene , camera , pSurfaceIntegrator , pSampler , renderer );
	}

	DrawScreenQuad( &mTextureSRV );

	ImGui::Render();

	HR( mSwapChain->Present( 1 , 0 ) );
}


void MeshViewApp::OnMouseDown( WPARAM buttonState , int x , int y )
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture( mhMainWnd );
}


void MeshViewApp::OnMouseUp( WPARAM buttonState , int x , int y )
{
	ReleaseCapture();
}


void MeshViewApp::OnMouseMove( WPARAM buttonState , int x , int y )
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;
}


void MeshViewApp::BuildScreenQuadGeometryBuffers()
{
	Vertex vertices[] =
	{
		{DirectX::XMFLOAT3( -1.0f, -1.0f, 0.5f ) , DirectX::XMFLOAT2( 0.0f, 1.0f ) } ,
		{ DirectX::XMFLOAT3( -1.0f, 1.0f, 0.5f ) , DirectX::XMFLOAT2( 0.0f, 0.0 ) } ,
		{ DirectX::XMFLOAT3( 1.0f, 1.0f, 0.5f ) , DirectX::XMFLOAT2( 1.0f, 0.0f ) } ,
		{ DirectX::XMFLOAT3( 1.0f, -1.0f, 0.5f ) , DirectX::XMFLOAT2( 1.0f , 1.0f ) }
	};

	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	VertexBufferDesc.ByteWidth = sizeof( Vertex ) * 4;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA VertexInitData;
	VertexInitData.pSysMem = &vertices[0];

	HR( md3dDevice->CreateBuffer( &VertexBufferDesc , &VertexInitData , &mScreenQuadVertexBuffer ) );

	UINT indices[6] =
	{
		0 , 1 , 2 ,
		0 , 2  , 3
	};

	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE; 
	IndexBufferDesc.ByteWidth = sizeof( UINT ) * 6;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA IndexInitData;
	IndexInitData.pSysMem = indices;

	HR( md3dDevice->CreateBuffer( &IndexBufferDesc , &IndexInitData , &mScreenQuadIndexBuffer ) );

}


void MeshViewApp::DrawScreenQuad( ID3D11ShaderResourceView* const * srv )
{
	UINT stride = sizeof( Vertex );
	UINT offset = 0;

	md3dImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	md3dImmediateContext->IASetVertexBuffers( 0 , 1 , &mScreenQuadVertexBuffer , &stride , &offset );
	md3dImmediateContext->IASetIndexBuffer( mScreenQuadIndexBuffer , DXGI_FORMAT_R32_UINT , 0 );

	mDebugTextureEffectFactory->SetVertexTransformConstantBuffer( md3dImmediateContext );

	// Set Shader State And Shader Object
	mDebugTextureEffectFactory->Apply( md3dImmediateContext );
	
	md3dImmediateContext->PSSetShaderResources( 0 , 1 , srv );
	md3dImmediateContext->PSSetSamplers( 0 , 1 , &mSampleLinear );

	md3dImmediateContext->DrawIndexed( 6 , 0 , 0 );
}


bool MeshViewApp::BuildEffectFactory()
{
	bool result = true;

	mDebugTextureEffectFactory = new DebugTextureEffectFactory( md3dDevice );
	if( !mDebugTextureEffectFactory )
	{
		return false;
	}

	result = mDebugTextureEffectFactory->Init( md3dDevice , "../Data/CSO/DebugTextureVertexShader.cso" , "../Data/CSO/DebugTexturePixelShader.cso" );
	if( !result )
	{
		return false;
	}

	return true;
}

void MeshViewApp::BuildResource()
{
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

	// --------------------------------------------------Create Random Texture-------------------------------------------------------
	//Create the random data
	srand( 13 );
	DirectX::XMFLOAT4* data = new DirectX::XMFLOAT4[Height * Width];
	for ( int i = 0; i < Width * Height; i++ )
	{
		data[i].x = ( double )rand() / ( double )RAND_MAX;
		data[i].y = ( double )rand() / ( double )RAND_MAX;
		data[i].z = ( double )rand() / ( double )RAND_MAX;
		data[i].w = 1.0f;
	}


	D3D11_TEXTURE2D_DESC tex_desc;
	ZeroMemory( &tex_desc , sizeof( tex_desc ) );
	tex_desc.Width = Width;
	tex_desc.Height = Height;
	tex_desc.MipLevels = 1;
	tex_desc.ArraySize = 1;
	tex_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.Usage = D3D11_USAGE_DYNAMIC;				// can be modify
	tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tex_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tex_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData , sizeof( D3D11_SUBRESOURCE_DATA ) );
	InitData.pSysMem = data;
	InitData.SysMemPitch = sizeof( data[0] ) * Width;

	HR( md3dDevice->CreateTexture2D( &tex_desc , &InitData , &mTexture ) );


	//Create the resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	ZeroMemory( &srv_desc , sizeof( srv_desc ) );
	srv_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = 1;
	srv_desc.Texture2D.MostDetailedMip = 0;

	HR( md3dDevice->CreateShaderResourceView( mTexture , &srv_desc , &mTextureSRV ) );
}

void MeshViewApp::ResizeTexture( int width , int height )
{
	ReleaseCOM( mTextureSRV );
	ReleaseCOM( mTexture );

	DirectX::XMFLOAT4* data = new DirectX::XMFLOAT4[height * width];
	for( int i = 0; i < width * height; i++ )
	{
		data[i].x = ( double )rand() / ( double )RAND_MAX;
		data[i].y = ( double )rand() / ( double )RAND_MAX;
		data[i].z = ( double )rand() / ( double )RAND_MAX;
		data[i].w = 1.0f;
	}


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

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData , sizeof( D3D11_SUBRESOURCE_DATA ) );
	InitData.pSysMem = data;
	InitData.SysMemPitch = sizeof( data[0] ) * width;

	HR( md3dDevice->CreateTexture2D( &tex_desc , &InitData , &mTexture ) );


	//Create the resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	ZeroMemory( &srv_desc , sizeof( srv_desc ) );
	srv_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = 1;
	srv_desc.Texture2D.MostDetailedMip = 0;

	HR( md3dDevice->CreateShaderResourceView( mTexture , &srv_desc , &mTextureSRV ) );
}