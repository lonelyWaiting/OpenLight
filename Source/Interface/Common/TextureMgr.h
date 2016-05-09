#ifndef TEXTUREMGR_H
#define TEXTUREMGR_H

#include "d3dUtil.h"
#include <map>

class TextureMgr
{
public:
	TextureMgr();
	~TextureMgr();

	void Init( ID3D11Device* device );

	ID3D11ShaderResourceView* CreateTexture( std::wstring filename );

private:
	TextureMgr( const TextureMgr& rhs );
	TextureMgr& operator = ( const TextureMgr& rhs );

private:
	ID3D11Device* md3dDevice;
	std::map<std::wstring , ID3D11ShaderResourceView*> mTextureSRV;
};

#endif