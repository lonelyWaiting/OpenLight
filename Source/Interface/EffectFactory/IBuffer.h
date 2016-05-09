#ifndef IBUFFER_H
#define IBUFFER_H

#include <Common/d3dUtil.h>

template<typename T>
class IBuffer
{
public:
    IBuffer() { }

    explicit IBuffer( _In_ ID3D11Device* device )
    {
        mBuffer = nullptr;

        Create( device );
    }

    ~IBuffer()
    {
        ReleaseCOM( mBuffer );
    }

public:
    void SetData( _In_ ID3D11DeviceContext* deviceContext , T const& value )
    {
        assert( mBuffer != 0 );

        D3D11_MAPPED_SUBRESOURCE InitData;

        HR( deviceContext->Map( mBuffer , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &InitData ) );

        *(T*)InitData.pData = value;

        deviceContext->Unmap( mBuffer , 0 );
    }

    ID3D11Buffer* const * GetBuffer()
    {
        return &mBuffer;
    }

private:
	void Create( _In_ ID3D11Device* device )
	{
		D3D11_BUFFER_DESC BufferDesc = { 0 };

		BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BufferDesc.ByteWidth = sizeof( T );
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BufferDesc.StructureByteStride = 0;
		BufferDesc.MiscFlags = 0;

		HR( device->CreateBuffer( &BufferDesc , nullptr , &mBuffer ) );
	}

private:
    ID3D11Buffer* mBuffer;

	// Prevent copying
    IBuffer( IBuffer const& rhs );
    IBuffer& operator= ( IBuffer const& rhs );
};

#endif