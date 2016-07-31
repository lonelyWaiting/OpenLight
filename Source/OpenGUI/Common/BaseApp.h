#ifndef D3DAPP_H
#define D3DAPP_H

#include "AppHelper.h"
#include <string>

#pragma comment(lib , "d3d11.lib")

class BaseApp
{
public:
	BaseApp(HINSTANCE hInstance);
	virtual ~BaseApp();

	HINSTANCE	GetInstance() const;
	HWND		GetHWnd() const;
	float		GetAspectRatio() const;

	int	Run();

	virtual bool	Init();
	virtual void	OnResize() = 0;
	virtual void	Update( float dt );
	virtual void	Render();

public:
	HRESULT MsgProc( HWND hwnd , UINT msg , WPARAM wParam , LPARAM lParam );

protected:
	bool InitMainWindow();
	bool InitDirect3D();

protected:
	HINSTANCE	mhAppInst;
	HWND		mhMainWnd;
	bool		mAppPaused;
	bool		mMinmized;
	bool		mMaximized;
	bool		mResizing;
	UINT		m4xMsaaQuality;

	ID3D11Device*           md3dDevice;
	ID3D11DeviceContext*    md3dImmediateContext;
	IDXGISwapChain*         mSwapChain;
	ID3D11Texture2D*        mDepthStencilBuffer;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT          mScreenViewport;

	std::wstring mMainWndCaption;
	D3D_DRIVER_TYPE md3dDriverType;
	int mClientWidth;
	int mClientHeight;
	bool mEnable4xMsaa;
};
#endif