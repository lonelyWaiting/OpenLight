#pragma once

#include "Common/BaseApp.h"

class MyApp : public BaseApp
{
public:
	MyApp( HINSTANCE hInstance );
	~MyApp();

public:
	bool Init();
	void OnResize();
	void Update( float dt );
	void Render();

private:
	void CreatelScreenQuadBuffer();

	void RenderToQuad( ID3D11ShaderResourceView* const * srv );

	bool InitRenderState();

	bool InitShader();
public:
	void ResizeRT(int width, int height, const float* data);

private:
	ID3D11Buffer* ScreenVertexBuffer;
	ID3D11Buffer* ScreenIndexBuffer;

	ID3D11ShaderResourceView* mRTSRV;
	ID3D11Texture2D* mRTResource;

	ID3D11InputLayout* mInputLayout;

	ID3D11SamplerState* mSampleLinear;

	ID3D11DepthStencilState* mLessEqual;

	ID3D11RasterizerState* mNoCullRS;

private:
	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;
};  