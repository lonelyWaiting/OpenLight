#include "d3dUtil.h"

ID3D11ShaderResourceView* d3dHelper::CreateTexture2DArraySRV(ID3D11Device* device,
															ID3D11DeviceContext* context,
															std::vector<std::wstring>& filenames)
{

	//-----------------------------------------第一步-----------------------------------
	//Load the texture elements individually from file. These texture
	//won't be used by the GPU (0 bind flags), they are just used to 
	//load the image from file. We use the STAGING usage so the CPU can
	//read the resource
	UINT size = filenames.size();
	std::vector<ID3D11Texture2D*> srcTex( size );		//要创建的纹理的数目

	for ( UINT i = 0; i < size; i++ )
	{
		//从文件中加载纹理对象
		HR( DirectX::CreateDDSTextureFromFileEx( device ,
												filenames[i].c_str() ,
												NULL ,
												D3D11_USAGE_IMMUTABLE ,
												D3D11_BIND_SHADER_RESOURCE ,
												NULL ,
												NULL ,
												false ,
												reinterpret_cast<ID3D11Resource**>( &srcTex[i] ) ,
												nullptr ) );
	}



	//----------------------------------------第二步---------------------------------------
	//Create the texture array. Each element in the texture
	//array has the same format/dimensions
	D3D11_TEXTURE2D_DESC texElementDesc;
	srcTex[0]->GetDesc( &texElementDesc );

	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = texElementDesc.Width;
	texArrayDesc.Height = texElementDesc.Height;
	texArrayDesc.MipLevels = texElementDesc.MipLevels;
	texArrayDesc.ArraySize = size;						//the size of texture array
	texArrayDesc.Format = texElementDesc.Format;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0;

	//create texture array
	ID3D11Texture2D* texArray = 0;
	HR( device->CreateTexture2D( &texArrayDesc , 0 , &texArray ) );



	//--------------------------------------第三步---------------------------------------------------
	//copy individual texture elements into texture array
	for ( UINT texElement = 0; texElement < size; texElement++ )
	{
		for ( UINT mipLevel = 0; mipLevel < texElementDesc.MipLevels; mipLevel++ )
		{
			context->CopySubresourceRegion( texArray ,
											D3D11CalcSubresource( mipLevel , texElement , texElementDesc.MipLevels ) ,
											NULL ,
											NULL ,
											NULL ,
											srcTex[texElement] ,
											mipLevel ,
											nullptr );
		}
	}



	//---------------------------------------第四步------------------------------------
	//create a resource view to the texture array
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = size;

	ID3D11ShaderResourceView* texArraySRV = 0;
	HR( device->CreateShaderResourceView( texArray , &viewDesc , &texArraySRV ) );


	//cleanup -- we only need the resource view
	ReleaseCOM( texArray );

	for ( UINT i = 0; i < size; i++ )
	{
		ReleaseCOM( srcTex[i] );
	}

	return texArraySRV;		//SRV: shader resource view
}



void ExtractFrustumPlanes(DirectX::XMFLOAT4 planes[6], DirectX::CXMMATRIX M)
{
	DirectX::XMFLOAT4X4 A;

	DirectX::XMStoreFloat4x4( &A , M );

	//
	//Left
	//
	planes[0].x = A._14 + A._11;
	planes[0].y = A._24 + A._21;
	planes[0].z = A._34 + A._31;
	planes[0].w = A._44 + A._41;

	//
	//Right
	//
	planes[1].x = A._14 - A._11;
	planes[1].y = A._24 - A._21;
	planes[1].z = A._34 - A._31;
	planes[1].w = A._44 - A._41;


	//
	// Bottom
	//
	planes[2].x = A._14 + A._12;
	planes[2].y = A._24 + A._22;
	planes[2].z = A._34 + A._32;
	planes[2].w = A._44 + A._42;

	//
	// Top
	//
	planes[3].x = A(0, 3) - A(0, 1);
	planes[3].y = A(1, 3) - A(1, 1);
	planes[3].z = A(2, 3) - A(2, 1);
	planes[3].w = A(3, 3) - A(3, 1);

	//
	// Near
	//
	planes[4].x = A(0, 2);
	planes[4].y = A(1, 2);
	planes[4].z = A(2, 2);
	planes[4].w = A(3, 2);

	//
	// Far
	//
	planes[5].x = A(0, 3) - A(0, 2);
	planes[5].y = A(1, 3) - A(1, 2);
	planes[5].z = A(2, 3) - A(2, 2);
	planes[5].w = A(3, 3) - A(3, 2);

	// Normalize the plane equations.
	for (int i = 0; i < 6; ++i)
	{
		DirectX::XMVECTOR v = DirectX::XMPlaneNormalize(DirectX::XMLoadFloat4(&planes[i]));
		XMStoreFloat4(&planes[i], v);
	}
}


ID3D11ShaderResourceView* d3dHelper::CreateRandomTexture1DSRV(ID3D11Device* device)
{
	//Create the random data
	DirectX::XMFLOAT4 randomValues[1024];

	for (int i = 0; i < 1024; i++)
	{
		randomValues[i].x = MathHelper::RandF(-1.0f, 1.0f);
		randomValues[i].y = MathHelper::RandF(-1.0f, 1.0f);
		randomValues[i].z = MathHelper::RandF(-1.0f, 1.0f);
		randomValues[i].w = MathHelper::RandF(-1.0f, 1.0f);
	}


	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = randomValues;
	initData.SysMemPitch = 1024 * sizeof(DirectX::XMFLOAT4);
	initData.SysMemSlicePitch = 0;

	//Create the texture
	D3D11_TEXTURE1D_DESC texDesc;
	texDesc.Width = 1024;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	texDesc.ArraySize = 1;

	ID3D11Texture1D* randomTex = 0;
	HR(device->CreateTexture1D(&texDesc, &initData, &randomTex));


	//Create the resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip = 0;

	ID3D11ShaderResourceView* randomTexSRV = 0;
	HR(device->CreateShaderResourceView(randomTex, &viewDesc, &randomTexSRV));

	//Cleanup
	ReleaseCOM(randomTex);

	return randomTexSRV;
}