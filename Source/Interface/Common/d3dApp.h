#ifndef D3DAPP_H
#define D3DAPP_H

#include "d3dUtil.h"
#include <string>

#pragma comment(lib , "d3d11.lib")

class D3DApp
{
public:
	D3DApp(HINSTANCE hInstance);
	virtual ~D3DApp();

	HINSTANCE	AppInst() const;
	HWND		MainWnd() const;
	float		AspectRatio() const;

	int Run();


	//Framework methods. Derived client class overrides these methods to
	//implement specific application requirements
	virtual bool Init();
	virtual void OnResize();
	virtual void UpdateScene(float dt) = 0;
	virtual void DrawScene() = 0;
	virtual LRESULT MsgProc(HWND hwnd , UINT msg , WPARAM wParam , LPARAM lParam);


	//convenience overrides for handling mouse input
	virtual void OnMouseDown(WPARAM buttonState , int x , int y) { };
	virtual void OnMouseUp(WPARAM buttonState , int x , int y) { };
	virtual void OnMouseMove(WPARAM buttonState , int x , int y) { };

	std::vector<byte> LoadShader( std::string filename );

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

	ID3D11Device* md3dDevice;
	ID3D11DeviceContext* md3dImmediateContext;
	IDXGISwapChain* mSwapChain;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT mScreenViewport;


	//Derived class should set these in derived constructor to customize starting values
	std::wstring mMainWndCaption;
	D3D_DRIVER_TYPE md3dDriverType;
	int mClientWidth;
	int mClientHeight;
	bool mEnable4xMsaa;
};
#endif