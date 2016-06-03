#pragma once

#include "Texture.h"

class ImageFile;

/*
	Format:
		<kd type="ImageTexture" filename="xx.png"/>
		kd为该纹理节点名，kd常用于表示漫反射纹理
		type为纹理class name
		value1，value2分别为棋盘纹理的两种颜色值
*/
class ImageTexture : public Texture
{
	DECLARE_DYNAMIC_CREATE_DERIVED( ImageTexture , Texture )

public:
	ImageTexture();

	~ImageTexture();

	virtual Spectrum Evalute( const Vector2f& uv , const Point3f& p ) const;

public:
	void Deserialization( tinyxml2::XMLElement* RootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );

private:
	char*  m_Filename;
	ImageFile* m_pImage;
};