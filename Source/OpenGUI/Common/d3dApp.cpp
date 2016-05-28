#include "d3dApp.h"
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

namespace
{
	D3DApp* gd3dApp = 0;
}


LRESULT CALLBACK MainWndProc(HWND hwnd , UINT msg , WPARAM wParam , LPARAM lParam)
{
	return gd3dApp->MsgProc(hwnd , msg , wParam , lParam);
}



D3DApp::D3DApp(HINSTANCE hInstance)
	:	mhAppInst(hInstance),
		mMainWndCaption(TEXT("D3D11 Application")),
		md3dDriverType(D3D_DRIVER_TYPE_HARDWARE),
		mClientWidth(800),
		mClientHeight(600),
		mEnable4xMsaa(false),
		mhMainWnd(NULL),
		mAppPaused(false),
		mMinmized(false),
		mMaximized(false),
		mResizing(false),
		m4xMsaaQuality(0),
		md3dDevice(NULL),
		md3dImmediateContext(NULL),
		mSwapChain(NULL),
		mDepthStencilBuffer(NULL),
		mRenderTargetView(NULL),
		mDepthStencilView(NULL)
{
	ZeroMemory(&mScreenViewport , sizeof(D3D11_VIEWPORT));

	gd3dApp = this;
}


D3DApp::~D3DApp()
{
	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilBuffer);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mSwapChain);

	if(md3dImmediateContext)
	{
		md3dImmediateContext->ClearState();
	}

	ReleaseCOM(md3dImmediateContext);
	ReleaseCOM(md3dDevice);
}



HINSTANCE D3DApp::AppInst() const
{
	return mhAppInst;
}


HWND D3DApp::MainWnd() const
{
	return mhMainWnd;
}

float D3DApp::AspectRatio() const
{
	return static_cast<float>((float)mClientWidth / (float)mClientHeight);
}


int D3DApp::Run()
{
	MSG msg = {0};

	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg , 0 , 0 , 0 , PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else
		{
			// 禁用暂停功能
			/*if(!mAppPaused)*/
			{
				UpdateScene(0);
				DrawScene();
			}
			/*else
			{
				Sleep(100);
			}*/
		}
	}

	return (int)msg.wParam;
}


bool D3DApp::Init()
{
	if(!InitMainWindow())
	{
		return false;
	}

	if(!InitDirect3D())
	{
		return false;
	}

	return true;
}

void D3DApp::OnResize()
{
	assert(md3dImmediateContext);
	assert(md3dDevice);
	assert(mSwapChain);


	//Release the old views , as they hold references to the buffers 
	//we will be destroying. Also release the old depth/stencil buffer.
	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mDepthStencilBuffer);


	//Resize the swap chain and recreate the render target view
	HR(mSwapChain->ResizeBuffers(1 , mClientWidth , mClientHeight , DXGI_FORMAT_R8G8B8A8_UNORM , 0));
	ID3D11Texture2D* backBuffer;	//get back buffer
	HR(mSwapChain->GetBuffer(0 , __uuidof(ID3D11Texture2D) , reinterpret_cast<void**>(&backBuffer)));
	HR(md3dDevice->CreateRenderTargetView(backBuffer , 0 , &mRenderTargetView));
	ReleaseCOM(backBuffer);

	//Create the depth/stencil buffer and view
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = mClientWidth;
	depthStencilDesc.Height = mClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (mEnable4xMsaa)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}

	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}


	depthStencilDesc.Usage			= D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags		= D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags		= 0;

	HR(md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer));
	HR(md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView));

	//Bind the render target view and depth/stencil view to the pipeline.
	md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);


	//Set the viewport transform
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<float>(mClientWidth);
	mScreenViewport.Height = static_cast<float>(mClientHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	md3dImmediateContext->RSSetViewports(1, &mScreenViewport);
}

extern LRESULT ImGui_ImplDX11_WndProcHandler( HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam );
LRESULT D3DApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if ( ImGui_ImplDX11_WndProcHandler( hwnd , msg , wParam , lParam ) )
		return true;

	switch (msg)
	{
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)	//切换到未激活状态
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
		mClientWidth = LOWORD(lParam);
		mClientHeight = HIWORD(lParam);

		if (md3dDevice)
		{
			if (wParam == SIZE_MINIMIZED)		//最小化的窗口
			{
				mAppPaused = true;
				mMinmized = true;
				mMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)	//最大化的窗口
			{
				mAppPaused = false;
				mMinmized = false;
				mMaximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)	//恢复
			{
				if (mMinmized)			//从最小化窗口恢复回之前的窗口
				{
					mAppPaused = false;
					mMinmized = false;
					OnResize();
				}
				else if (mMaximized)
				{
					mAppPaused = false;
					mMaximized = false;
					OnResize();
				}
				else if (mResizing)
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else
				{
					OnResize();

					//ImGui_ImplDX11_CreateDeviceObjects();
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

	//WM_EXITSIZEMOVE is sent when the user release the resize bars.
	//Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
	{
		mAppPaused = false;
		mResizing = false;
		OnResize();
		return 0;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_MENUCHAR:
	{
		return MAKELRESULT(0, MNC_CLOSE);
	}

	case WM_GETMINMAXINFO:
	{
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;
	}

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	{
		OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	{
		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}

	case WM_MOUSEMOVE:
	{
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}

	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool D3DApp::InitMainWindow()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mhAppInst;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = TEXT("D3DWndClassName");

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, TEXT("Register Class Failed."), TEXT("ERROR"), MB_OK);
		return false;
	}

	//RECT R = { 0, 0, mClientWidth, mClientHeight };
	//int width = R.right - R.left;
	//int height = R.bottom - R.top;

	mhMainWnd = CreateWindow(TEXT("D3DWndClassName"),
							mMainWndCaption.c_str(),
							WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, CW_USEDEFAULT,
							mClientWidth, mClientHeight,
							0, 0,
							mhAppInst,
							0);
	if (!mhMainWnd)
	{
		MessageBox(NULL, TEXT("Create Window Failed."), TEXT("ERROR"), MB_OK);
		return false;
	}

	ShowWindow(mhMainWnd, SW_SHOW);
	UpdateWindow(mhMainWnd);

	AllocConsole();
	SetConsoleTitle( _T( "Debug Output" ) );
	freopen( "CONOUT$" , "w" , stdout );

	return true;
}

bool D3DApp::InitDirect3D()
{
	UINT createDeviceFlags = 0;

	D3D_FEATURE_LEVEL featrueLevel;

	HRESULT hr = D3D11CreateDevice(
			0,							//default adapter
			md3dDriverType,
			NULL,						//no software device
			createDeviceFlags,
			0, 0,						//default feature level array
			D3D11_SDK_VERSION,
			&md3dDevice,
			&featrueLevel,
			&md3dImmediateContext);
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("Direct3D Feature Level 11 unsupported."), TEXT("ERROR"), MB_OK);
		return false;
	}

	if (featrueLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(NULL, TEXT("Direct3D Feature Level 11 unsupported."), 0, 0);
		return false;
	}


	HR(md3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));
	assert(m4xMsaaQuality > 0);

	//Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = mClientWidth;
	sd.BufferDesc.Height = mClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//use 4X MASS?
	if (mEnable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m4xMsaaQuality - 1;
	}

	//No MASS
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


	//To correctly create the swap chain , we must use the IDXGIFactory that was
	//used to create the device. If we tried to use a different IDXGIFactory instance
	//(by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain:
	//This function is being called with a device from a different IDXGIFactory."
	IDXGIDevice* dxgiDevice = NULL;
	HR(md3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = NULL;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = NULL;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	HR(dxgiFactory->CreateSwapChain(md3dDevice , &sd , &mSwapChain));

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	
	OnResize();

	return true;
}

std::vector<byte> D3DApp::LoadShader( std::string filename )
{
	std::vector<byte> FileData;

	std::ifstream ShaderFile( filename , std::ios::in | std::ios::binary | std::ios::ate );

	if( ShaderFile.is_open() )
	{
		int Length = ( int )ShaderFile.tellg();

		FileData.resize( Length );

		ShaderFile.seekg( 0 , std::ios::beg );
		ShaderFile.read( reinterpret_cast< char* >( &FileData[0] ) , FileData.size() );
		ShaderFile.close();
	}

	return FileData;
}