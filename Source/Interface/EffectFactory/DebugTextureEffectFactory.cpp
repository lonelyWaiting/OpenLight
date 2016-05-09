#include "DebugTextureEffectFactory.h"

DebugTextureEffectFactory::DebugTextureEffectFactory( _In_ ID3D11Device* device )
	:IEffect() ,
	mVertexTransformConstantBuffer( device )
{
    mVertexShader = nullptr;
    mInputLayout = nullptr;

    mInputLayout = nullptr;

	mSampleLinear = nullptr;
}

DebugTextureEffectFactory::~DebugTextureEffectFactory()
{
    ReleaseCOM( mVertexShader );
    ReleaseCOM( mPixelShader );

    ReleaseCOM( mInputLayout );

	ReleaseCOM( mSampleLinear );
}

bool DebugTextureEffectFactory::Init( ID3D11Device* device , std::string VertexShaderFilename , std::string PixelShaderFilename )
{
    return CreateDebugTextureShader( device , VertexShaderFilename , PixelShaderFilename );
}

bool DebugTextureEffectFactory::CreateDebugTextureShader( ID3D11Device* device , std::string VertexShaderFilename , std::string PixelShaderFilename )
{
    HRESULT result;

    std::vector<byte> VSFile = LoadShader( VertexShaderFilename );
    std::vector<byte> PSFile = LoadShader( PixelShaderFilename );

    result = device->CreateVertexShader( VSFile.data() , VSFile.size() , nullptr , &mVertexShader );
    if ( FAILED( result ) )
    {
        return false;
    }

    result = device->CreatePixelShader( PSFile.data() , PSFile.size() , nullptr , &mPixelShader );
    if ( FAILED( result ) )
    {
        return false;
    }


    D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[] = 
    {
        { "POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0  , 0 , D3D11_INPUT_PER_VERTEX_DATA , 0 } , 
        { "TEXCOORD" , 0 , DXGI_FORMAT_R32G32_FLOAT , 0 , 12 , D3D11_INPUT_PER_VERTEX_DATA , 0 }
    };

    UINT size = ARRAYSIZE( InputLayoutDesc );

    result = device->CreateInputLayout( InputLayoutDesc , size , VSFile.data() , VSFile.size() , &mInputLayout );
    if ( FAILED( result ) )
    {
        return false;
    }

    return true;
}


void DebugTextureEffectFactory::Apply( ID3D11DeviceContext* deviceContext )
{
	deviceContext->IASetInputLayout( mInputLayout );

    deviceContext->VSSetShader( mVertexShader , nullptr , 0 );
    deviceContext->PSSetShader( mPixelShader , nullptr , 0 );

	deviceContext->VSSetConstantBuffers( 0 , 1 , mVertexTransformConstantBuffer.GetBuffer() );
}

void DebugTextureEffectFactory::SetVertexTransformConstantBuffer( ID3D11DeviceContext* deviceContext , DirectX::CXMMATRIX WorldViewProj )
{
    DebugTextureEffect::VertexShader::cbPerFrame value;
    DirectX::XMStoreFloat4x4( &value.WorldViewProj , DirectX::XMMatrixTranspose( WorldViewProj ) );

    mVertexTransformConstantBuffer.SetData( deviceContext , value );
}