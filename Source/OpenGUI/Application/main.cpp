#include <Common/d3dApp.h>
#include <imgui.h>
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>

#include "OpenLight.h"

struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 Tex;
};

class MyApp : public D3DApp
{
public:
	MyApp( HINSTANCE hInstance );
	~MyApp();

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
public:
	void ResizeTexture( int width , int height );

private:
	ID3D11Buffer* ScreenVertexBuffer;
	ID3D11Buffer* ScreenIndexBuffer;

	ID3D11ShaderResourceView* mTextureSRV;
	ID3D11Texture2D* mTexture;

	ID3D11VertexShader* pVertexShader;
	ID3D11PixelShader* pPixelShader;

	ID3D11InputLayout* mInputLayout;

	ID3D11SamplerState* mSampleLinear;

	ID3D11DepthStencilState* mLessEqual;

	ID3D11RasterizerState* mNoCullRS;

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

	MyApp theApp( hInstance );

	if( !theApp.Init() )
		return 0;

	return theApp.Run();
}


MyApp::MyApp( HINSTANCE hInstance )
	: D3DApp( hInstance )
	, ScreenVertexBuffer( nullptr )
	, ScreenIndexBuffer( nullptr )
	, mSampleLinear( nullptr )
	, mTexture( nullptr )
	, mTextureSRV( nullptr )
	, Width( 0 )
	, Height( 0 )
	, fov( 0.0f )
	, spp( 0 )
	, MaxDepth( 0 )
	, bResize( false )
	, mLessEqual( nullptr )
	, mNoCullRS( nullptr )
	, pVertexShader( nullptr )
	, pPixelShader( nullptr )
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


MyApp::~MyApp()
{
	ReleaseCOM( ScreenVertexBuffer );
	ReleaseCOM( ScreenIndexBuffer );

	ReleaseCOM( mSampleLinear );

	SafeDelete( scene );

	SafeDelete( camera );

	SafeDelete( pSurfaceIntegrator );

	SafeDelete( pSampler );

	SafeDelete( renderer );
}


bool MyApp::Init()
{
	if( !D3DApp::Init() )
	{
		return false;
	}

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
	}

	{
		HRESULT result;
		ID3DBlob* pVertexBlob;
		D3DCompileFromFile( L"Shader/DebugTextureVertexShader.hlsl" , nullptr , nullptr , "main" , "vs_4_0" , 0 , 0 , &pVertexBlob , nullptr );
		//std::vector<byte>& VSFile = LoadShader( "../Data/CSO/DebugTextureVertexShader.cso" );

		result = md3dDevice->CreateVertexShader( pVertexBlob->GetBufferPointer() , pVertexBlob->GetBufferSize() , nullptr , &pVertexShader );
		if( FAILED( result ) )
		{
			return false;
		}

		ID3DBlob* pPixelBlob;
		//std::vector<byte>& PSFile = LoadShader( "../Data/CSO/DebugTexturePixelShader.cso" );
		D3DCompileFromFile( L"Shader/DebugTexturePixelShader.hlsl" , nullptr , nullptr , "main" , "ps_4_0" , 0 , 0 , &pPixelBlob , nullptr );
		result = md3dDevice->CreatePixelShader( pPixelBlob->GetBufferPointer() , pPixelBlob->GetBufferSize() , nullptr , &pPixelShader );
		if( FAILED( result ) )
		{
			return false;
		}


		D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] =
		{
			{ "POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0  , 0 , D3D11_INPUT_PER_VERTEX_DATA , 0 } ,
			{ "TEXCOORD" , 0 , DXGI_FORMAT_R32G32_FLOAT , 0 , 12 , D3D11_INPUT_PER_VERTEX_DATA , 0 }
		};

		UINT size = ARRAYSIZE( InputLayoutDesc );

		result = md3dDevice->CreateInputLayout( InputLayoutDesc , size , /*VSFile.data() , VSFile.size()*/pVertexBlob->GetBufferPointer() , pVertexBlob->GetBufferSize() , &mInputLayout );
		if( FAILED( result ) )
		{
			return false;
		}
	}

	
	{
		Vertex vertices[] =
		{
			{ DirectX::XMFLOAT3( -1.0f, -1.0f, 0.5f ) , DirectX::XMFLOAT2( 0.0f, 1.0f ) } ,
			{ DirectX::XMFLOAT3( -1.0f, 1.0f, 0.5f ) , DirectX::XMFLOAT2( 0.0f, 0.0 ) } ,
			{ DirectX::XMFLOAT3( 1.0f , 1.0f, 0.5f ) , DirectX::XMFLOAT2( 1.0f, 0.0f ) } ,
			{ DirectX::XMFLOAT3( 1.0f , -1.0f, 0.5f ) , DirectX::XMFLOAT2( 1.0f , 1.0f ) }
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

	{
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


			
		}
	}

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

	Width    = camera->GetFilm()->GetResolution().x;
	Height   = camera->GetFilm()->GetResolution().y;
	fov      = camera->GetFovy();
	spp      = renderer->GetSpp();
	MaxDepth = pSurfaceIntegrator->GetMaxRecusiveDepth();

	ResizeTexture( Width , Height );

	HANDLE handle = GetStdHandle( STD_OUTPUT_HANDLE );
	SetConsoleTextAttribute( handle , FOREGROUND_INTENSITY | FOREGROUND_GREEN );
	printf( "Image Width  : ");
	SetConsoleTextAttribute( handle , FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );
	printf( "%d\n" , Width );
	SetConsoleTextAttribute( handle , FOREGROUND_INTENSITY | FOREGROUND_GREEN );
	printf( "Image Height : ");
	SetConsoleTextAttribute( handle , FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );
	printf( "%d\n" , Height );
	SetConsoleTextAttribute( handle , FOREGROUND_INTENSITY | FOREGROUND_GREEN );
	printf( "Camera Fov : ");
	SetConsoleTextAttribute( handle , FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );
	printf( "%f\n" , fov );
	SetConsoleTextAttribute( handle , FOREGROUND_INTENSITY | FOREGROUND_GREEN );
	printf( "Renderer Sample Per Pixel : ");
	SetConsoleTextAttribute( handle , FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );
	printf( "%d\n" , spp );
	SetConsoleTextAttribute( handle , FOREGROUND_INTENSITY | FOREGROUND_GREEN );
	printf( "Max Ray Tracing Recusive Depth : ");
	SetConsoleTextAttribute( handle , FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );
	printf( "%d\n\n" , MaxDepth );

	return true;
}


void MyApp::OnResize()
{
	D3DApp::OnResize();
}


void MyApp::UpdateScene( float dt )
{

}

void MyApp::DrawScene()
{
	float Yellow[4]= { 0.75 , 0.75 , 0.75 , 1.0 };
	md3dImmediateContext->ClearRenderTargetView( mRenderTargetView , reinterpret_cast< const float* >( Yellow ) );
	md3dImmediateContext->ClearDepthStencilView( mDepthStencilView , D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL , 1.0f , 0 );

	md3dImmediateContext->RSSetState( mNoCullRS );

	md3dImmediateContext->OMSetDepthStencilState( mLessEqual , 0 );


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

		if( ImGui::Button( "Begin Rending" ) )
		{
			renderer->ResetRender();
		}

		if( ImGui::Button( "Save Scene" ) )	bSave ^= 1;
	}

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

			D3D11_MAPPED_SUBRESOURCE MappedRes;
			ZeroMemory( &MappedRes , sizeof( D3D11_MAPPED_SUBRESOURCE ) );
			md3dImmediateContext->Map( mTexture , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &MappedRes );
			float* pdata = static_cast< float* >( MappedRes.pData );
			Image* image = camera->GetFilm()->GetImage();
			for( int row = 0; row < Height; row++ )
			{
				for( int col = 0; col < Width; col++ )
				{
					Spectrum sourcedata = image->GetColor( row , col );
					double rgb[3];
					sourcedata.ToRGB( rgb );

					pdata[4 * col + 0] = ( float )rgb[0];
					pdata[4 * col + 1] = ( float )rgb[1];
					pdata[4 * col + 2] = ( float )rgb[2];
					pdata[4 * col + 3] = 1.0f;
				}

				pdata = ( float* )( ( BYTE* )pdata + MappedRes.RowPitch );
			}
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


void MyApp::OnMouseDown( WPARAM buttonState , int x , int y )
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture( mhMainWnd );
}


void MyApp::OnMouseUp( WPARAM buttonState , int x , int y )
{
	ReleaseCapture();
}


void MyApp::OnMouseMove( WPARAM buttonState , int x , int y )
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void MyApp::DrawScreenQuad( ID3D11ShaderResourceView* const * srv )
{
	UINT stride = sizeof( Vertex );
	UINT offset = 0;

	md3dImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	md3dImmediateContext->IASetVertexBuffers( 0 , 1 , &ScreenVertexBuffer , &stride , &offset );
	md3dImmediateContext->IASetIndexBuffer( ScreenIndexBuffer , DXGI_FORMAT_R32_UINT , 0 );

	md3dImmediateContext->IASetInputLayout( mInputLayout );

	md3dImmediateContext->VSSetShader( pVertexShader , nullptr , 0 );
	md3dImmediateContext->PSSetShader( pPixelShader , nullptr , 0 );
	
	md3dImmediateContext->PSSetShaderResources( 0 , 1 , srv );
	md3dImmediateContext->PSSetSamplers( 0 , 1 , &mSampleLinear );

	md3dImmediateContext->DrawIndexed( 6 , 0 , 0 );
}

void MyApp::ResizeTexture( int width , int height )
{
	ReleaseCOM( mTextureSRV );
	ReleaseCOM( mTexture );

	mTexture = nullptr;

	DirectX::XMFLOAT4* data = new DirectX::XMFLOAT4[height * width];
	for( int i = 0; i < width; i++ )
	{
		for( int j = 0; j < height; j++ )
		{
			data[i * height + j].x = ( ( i & 1 ) || ( j & 1 ) ) ? 0.0f : 1.0f;
			data[i * height + j].y = ( ( i & 1 ) || ( j & 1 ) ) ? 0.0f : 1.0f;
			data[i * height + j].z = ( ( i & 1 ) || ( j & 1 ) ) ? 0.0f : 1.0f;
			data[i * height + j].w = 1.0f;
		}
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
	InitData.SysMemPitch = sizeof( DirectX::XMFLOAT4 ) * width;

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