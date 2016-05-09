#include "RenderState.h"

ID3D11RasterizerState* RenderStates::mWireframeRS = NULL;
ID3D11RasterizerState* RenderStates::mNoCullRS = NULL;

ID3D11BlendState* RenderStates::mAlphaToCoverageBS = NULL;
ID3D11BlendState* RenderStates::mTransparentBS = NULL;

ID3D11DepthStencilState* RenderStates::mLessEqual = NULL;
ID3D11DepthStencilState* RenderStates::mEqualsDSS = NULL;

void RenderStates::InitAll( ID3D11Device* device )
{
    D3D11_RASTERIZER_DESC WireframeDesc;
    ZeroMemory( &WireframeDesc , sizeof( D3D11_RASTERIZER_DESC ) );
    WireframeDesc.FillMode = D3D11_FILL_WIREFRAME;			//线框填充
    WireframeDesc.CullMode = D3D11_CULL_BACK;				//背面剔除
    WireframeDesc.FrontCounterClockwise = false;			//顺时针为正面
    WireframeDesc.DepthClipEnable = true;					//启用深度剪裁

    HR( device->CreateRasterizerState( &WireframeDesc , &mWireframeRS ) );



    D3D11_RASTERIZER_DESC NoCullDesc;
    ZeroMemory( &NoCullDesc , sizeof( D3D11_RASTERIZER_DESC ) );
    NoCullDesc.FillMode = D3D11_FILL_SOLID;					//实体填充
    NoCullDesc.CullMode = D3D11_CULL_NONE;					//不剔除
    NoCullDesc.FrontCounterClockwise = false;				//顺时针为正面
    NoCullDesc.DepthClipEnable = true;						//启用深度剪裁

    HR( device->CreateRasterizerState( &NoCullDesc , &mNoCullRS ) );



    D3D11_BLEND_DESC alphaToCoverageDesc = { 0 };
    alphaToCoverageDesc.AlphaToCoverageEnable = true;
    alphaToCoverageDesc.IndependentBlendEnable = false;
    alphaToCoverageDesc.RenderTarget[0].BlendEnable = false;
    alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    HR( device->CreateBlendState( &alphaToCoverageDesc , &mAlphaToCoverageBS ) );

    //
    // TransparentBS
    //

    D3D11_BLEND_DESC transparentDesc = { 0 };
    transparentDesc.AlphaToCoverageEnable = false;
    transparentDesc.IndependentBlendEnable = false;

    transparentDesc.RenderTarget[0].BlendEnable = true;
    transparentDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    transparentDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    transparentDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    transparentDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    transparentDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    HR( device->CreateBlendState( &transparentDesc , &mTransparentBS ) );


    D3D11_DEPTH_STENCIL_DESC LessEqualDepthStencilDesc;
    LessEqualDepthStencilDesc.DepthEnable = true;
    LessEqualDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    LessEqualDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    LessEqualDepthStencilDesc.StencilEnable = false;

    HR( device->CreateDepthStencilState( &LessEqualDepthStencilDesc , &mLessEqual ) );


	D3D11_DEPTH_STENCIL_DESC EqualsDepthStencilDesc;
	ZeroMemory( &EqualsDepthStencilDesc , sizeof( D3D11_DEPTH_STENCIL_DESC ) );
	EqualsDepthStencilDesc.DepthEnable = true;
	EqualsDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	EqualsDepthStencilDesc.DepthFunc = D3D11_COMPARISON_EQUAL;

	HR( device->CreateDepthStencilState( &EqualsDepthStencilDesc , &mEqualsDSS ) );
}


void RenderStates::DestroyAll()
{
    ReleaseCOM( mWireframeRS );
    ReleaseCOM( mNoCullRS );

    ReleaseCOM( mAlphaToCoverageBS );
    ReleaseCOM( mTransparentBS );

    ReleaseCOM( mLessEqual );
	ReleaseCOM( mEqualsDSS );
}