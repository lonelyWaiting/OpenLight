#include "BaseApp.h"
#include <Windowsx.h>
#include <sstream>

#include <imgui.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>

#include "stdio.h"
#include "io.h"
#include "fcntl.h"

#include "ImModule.h"
#include "AssetViewer.h"

namespace
{
	BaseApp* gd3dApp = 0;
}

LRESULT CALLBACK MainWndProc( HWND hwnd , UINT msg , WPARAM wParam , LPARAM lParam )
{
	return gd3dApp->MsgProc( hwnd , msg , wParam , lParam );
}

BaseApp::BaseApp( HINSTANCE hInstance )
	:	mhAppInst( hInstance ) ,
		mMainWndCaption( TEXT( "OpenLight By Liuteng" ) ) ,
		md3dDriverType( D3D_DRIVER_TYPE_HARDWARE ) ,
		mClientWidth( 1024 ) ,
		mClientHeight( 768 ) ,
		mEnable4xMsaa( false ) ,
		mhMainWnd( NULL ) ,
		mAppPaused( false ) ,
		mMinmized( false ) ,
		mMaximized( false ) ,
		mResizing( false ) ,
		m4xMsaaQuality( 0 ) ,
		md3dDevice( NULL ) ,
		md3dImmediateContext( NULL ) ,
		mSwapChain( NULL ) ,
		mDepthStencilBuffer( NULL ) ,
		mRenderTargetView( NULL ) ,
		mDepthStencilView( NULL )
{
	ZeroMemory( &mScreenViewport , sizeof( D3D11_VIEWPORT ) );

	gd3dApp = this;
}


BaseApp::~BaseApp()
{
	ReleaseCOM( mRenderTargetView );
	ReleaseCOM( mDepthStencilBuffer );
	ReleaseCOM( mDepthStencilView );
	ReleaseCOM( mSwapChain );

	if( md3dImmediateContext )
	{
		md3dImmediateContext->ClearState();
	}

	ReleaseCOM( md3dImmediateContext );
	ReleaseCOM( md3dDevice );

	ImShutdown();
}

HINSTANCE BaseApp::GetInstance() const
{
	return mhAppInst;
}

HWND BaseApp::GetHWnd() const
{
	return mhMainWnd;
}

float BaseApp::GetAspectRatio() const
{
	return static_cast< float >( ( float )mClientWidth / ( float )mClientHeight );
}


int BaseApp::Run()
{
	MSG msg = { 0 };

	while( msg.message != WM_QUIT )
	{
		if( PeekMessage( &msg , 0 , 0 , 0 , PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			ImNewFrame();
			Update( 0 );
			Render();
			ImRenderDrawData();
			mSwapChain->Present(1, 0);
		}
	}

	return ( int )msg.wParam;
}

void BaseApp::Render()
{
	// Base Class Implement , Nothing to do
}

void BaseApp::Update( float dt )
{
	// Base Class Implement , Nothing to do
}

bool BaseApp::Init()
{
	if( !InitMainWindow() )
	{
		return false;
	}

	if( !InitDirect3D() )
	{
		return false;
	}

	ImInit( mhMainWnd , md3dDevice , md3dImmediateContext );

	ShowWindow( mhMainWnd , SW_SHOWDEFAULT );
	UpdateWindow( mhMainWnd );

	return true;
}

extern LRESULT ImWndProHandler( HWND hwnd , UINT msg , WPARAM wParam , LPARAM lParam );

HRESULT BaseApp::MsgProc( HWND hwnd , UINT msg , WPARAM wParam , LPARAM lParam )
{
	if( ImWndProHandler( hwnd , msg , wParam , lParam ) )
	{
		return true;
	}

	switch( msg )
	{
		case WM_ACTIVATE:
		{
			if( LOWORD( wParam ) == WA_INACTIVE )	//切换到未激活状态
			{
				mAppPaused = true;
			}
			else								//窗口切换到激活状态
			{
				mAppPaused = false;
			}

			return 0;
		}

		case WM_SIZE:							//窗口大小被改变
		{
			mClientWidth = LOWORD( lParam );
			mClientHeight = HIWORD( lParam );

			if( md3dDevice )
			{
				if( wParam == SIZE_MINIMIZED )		//最小化的窗口
				{
					mAppPaused = true;
					mMinmized = true;
					mMaximized = false;
				}
				else if( wParam == SIZE_MAXIMIZED )	//最大化的窗口
				{
					mAppPaused = false;
					mMinmized = false;
					mMaximized = true;
					OnResize();
				}
				else if( wParam == SIZE_RESTORED )	//恢复
				{
					if( mMinmized )			//从最小化窗口恢复回之前的窗口
					{
						mAppPaused = false;
						mMinmized = false;
						OnResize();
					}
					else if( mMaximized )
					{
						mAppPaused = false;
						mMaximized = false;
						OnResize();
					}
					else if( mResizing )
					{
					}
					else
					{
						OnResize();
					}
				}
			}

			return 0;
		}

		//WM_ENTERSIZEMOVE is sent when user grabs the resize bars.
		case WM_ENTERSIZEMOVE:
		{
			mAppPaused = true;
			mResizing = true;
			return 0;
		}

		case WM_EXITSIZEMOVE:
		{
			mAppPaused = false;
			mResizing = false;
			OnResize();
			return 0;
		}

		case WM_DESTROY:
		{
			PostQuitMessage( 0 );
			return 0;
		}

		case WM_MENUCHAR:
		{
			return MAKELRESULT( 0 , MNC_CLOSE );
		}

		case WM_GETMINMAXINFO:
		{
			( ( MINMAXINFO* )lParam )->ptMinTrackSize.x = 200;
			( ( MINMAXINFO* )lParam )->ptMinTrackSize.y = 200;
			return 0;
		}

		case WM_DROPFILES:
		{
			HDROP hDrop = ( HDROP )( wParam );
			TCHAR lpszFile[MAX_PATH];

			UINT nCount = DragQueryFile( hDrop , 0xFFFFFFFF , NULL , 0 );

			for( UINT i = 0; i < nCount; i++ )
			{
				if( DragQueryFile( hDrop , i , lpszFile , MAX_PATH ) > 0 )
				{
					AssetViewer::LoadAsset( lpszFile );
				}
			}

			DragFinish( hDrop );

			return 0;
		}
	}

	return DefWindowProc( hwnd , msg , wParam , lParam );
}

bool BaseApp::InitMainWindow()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mhAppInst;
	wc.hIcon = LoadIcon( 0 , IDI_APPLICATION );
	wc.hCursor = LoadCursor( 0 , IDC_ARROW );
	wc.hbrBackground = ( HBRUSH )GetStockObject( NULL_BRUSH );
	wc.lpszMenuName = 0;
	wc.lpszClassName = TEXT( "OpenLight" );

	if( !RegisterClass( &wc ) )
	{
		MessageBox( NULL , TEXT( "Register Class Failed." ) , TEXT( "ERROR" ) , MB_OK );
		return false;
	}

	mhMainWnd = CreateWindowEx( WS_EX_ACCEPTFILES ,
								_T( "OpenLight" ) ,
								mMainWndCaption.c_str() ,
								WS_OVERLAPPEDWINDOW ,
								CW_USEDEFAULT , CW_USEDEFAULT ,
								mClientWidth , mClientHeight ,
								0 , 0 ,
								mhAppInst ,
								0 );
	if( !mhMainWnd )
	{
		MessageBox( NULL , TEXT( "Create Window Failed." ) , TEXT( "ERROR" ) , MB_OK );
		return false;
	}

	AllocConsole();
	SetConsoleTitle( _T( "Debug Output" ) );
	FILE* stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);

	return true;
}

bool BaseApp::InitDirect3D()
{
	UINT createDeviceFlags = 0;

	D3D_FEATURE_LEVEL featrueLevel;

	HRESULT hr = D3D11CreateDevice( 0 ,							//default adapter
									md3dDriverType ,
									NULL ,						//no software device
									createDeviceFlags ,
									0 , 0 ,						//default feature level array
									D3D11_SDK_VERSION ,
									&md3dDevice ,
									&featrueLevel ,
									&md3dImmediateContext );
	if( FAILED( hr ) )
	{
		MessageBox( NULL , TEXT( "Direct3D Feature Level 11 unsupported." ) , TEXT( "ERROR" ) , MB_OK );
		return false;
	}

	if( featrueLevel != D3D_FEATURE_LEVEL_11_0 )
	{
		MessageBox( NULL , TEXT( "Direct3D Feature Level 11 unsupported." ) , 0 , 0 );
		return false;
	}


	HR( md3dDevice->CheckMultisampleQualityLevels( DXGI_FORMAT_R8G8B8A8_UNORM , 4 , &m4xMsaaQuality ) );
	assert( m4xMsaaQuality > 0 );

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = mClientWidth;
	sd.BufferDesc.Height = mClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if( mEnable4xMsaa )
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = mhMainWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	IDXGIDevice* dxgiDevice = NULL;
	HR( md3dDevice->QueryInterface( __uuidof( IDXGIDevice ) , ( void** )&dxgiDevice ) );

	IDXGIAdapter* dxgiAdapter = NULL;
	HR( dxgiDevice->GetParent( __uuidof( IDXGIAdapter ) , ( void** )&dxgiAdapter ) );

	IDXGIFactory* dxgiFactory = NULL;
	HR( dxgiAdapter->GetParent( __uuidof( IDXGIFactory ) , ( void** )&dxgiFactory ) );

	HR( dxgiFactory->CreateSwapChain( md3dDevice , &sd , &mSwapChain ) );

	ReleaseCOM( dxgiDevice );
	ReleaseCOM( dxgiAdapter );
	ReleaseCOM( dxgiFactory );

	//OnResize();

	return true;
}