#ifndef RENDERSTATE_H
#define RENDERSTATE_H

#include "d3dUtil.h"

class RenderStates
{
public:
    static void InitAll( ID3D11Device* device );
    static void DestroyAll();

    // Rasterizer states
    static ID3D11RasterizerState* mWireframeRS;
    static ID3D11RasterizerState* mNoCullRS;

    // Blend states
    static ID3D11BlendState* mAlphaToCoverageBS;
    static ID3D11BlendState* mTransparentBS;

    static ID3D11DepthStencilState* mLessEqual;
	static ID3D11DepthStencilState* mEqualsDSS;
};

#endif