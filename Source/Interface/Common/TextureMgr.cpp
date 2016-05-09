#include "TextureMgr.h"

TextureMgr::TextureMgr()
	: md3dDevice( nullptr )
{

}

TextureMgr::~TextureMgr()
{
	for( auto it = mTextureSRV.begin(); it != mTextureSRV.end(); it++ )
	{
		ReleaseCOM( it->second );
	}

	mTextureSRV.clear();
}


void TextureMgr::Init( ID3D11Device* device )
{
	md3dDevice = device;
}


ID3D11ShaderResourceView* TextureMgr::CreateTexture( std::wstring filename )
{
	ID3D11ShaderResourceView* srv = 0;

	if( mTextureSRV.find( filename ) != mTextureSRV.end() )
	{
		srv = mTextureSRV[filename];
	}
	else
	{
		HR( DirectX::CreateDDSTextureFromFile( md3dDevice , filename.c_str() , nullptr , &srv ) );
		
		mTextureSRV[filename] = srv;
	}

	return srv;
}