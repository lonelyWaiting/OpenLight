#ifndef DEBUG_TEXTURE_EFFECT_FACTORY_H
#define DEBUG_TEXTURE_EFFECT_FACTORY_H

#include <Common/IEffect.h>
#include <Common/d3dUtil.h>
#include "Vertex.h"
#include "IBuffer.h"

class DebugTextureEffectFactory : public IEffect
{
public:
	explicit DebugTextureEffectFactory( _In_ ID3D11Device* device );
    ~DebugTextureEffectFactory();

public:
    bool Init( ID3D11Device* device , std::string VertexShaderFilename , std::string PixelShaderFilename );

	void SetVertexTransformConstantBuffer( ID3D11DeviceContext* deviceContext , DirectX::CXMMATRIX WorldViewProj = DirectX::XMMatrixIdentity() );

    void Apply( ID3D11DeviceContext* deviceContext );

private:
    bool CreateDebugTextureShader( ID3D11Device* device , std::string VertexShaderFilename , std::string PixelShaderFilename );

	void CreateSampler( ID3D11Device* device );

private:
    ID3D11VertexShader* mVertexShader;
    ID3D11PixelShader* mPixelShader;

    ID3D11InputLayout* mInputLayout;

	ID3D11SamplerState* mSampleLinear;

    IBuffer<DebugTextureEffect::VertexShader::cbPerFrame> mVertexTransformConstantBuffer;
};
#endif