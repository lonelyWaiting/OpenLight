#pragma once

#include "Texture.h"

class ConstantTexture;

/*
	Format:
		<kd type="CheckboardTexture" value1="1.0,1.0,1.0" value2="0.0,0.0,0.0"/>
	kd为该纹理节点名，kd常用于表示漫反射纹理
	type为纹理class name
	value1，value2分别为棋盘纹理的两种颜色值
*/
class CheckboardTexture : public Texture
{
	DECLARE_DYNAMIC_CREATE_DERIVED( CheckboardTexture , Texture )

public:
	CheckboardTexture();

	CheckboardTexture( ConstantTexture* tex1 , ConstantTexture* tex2 );

	~CheckboardTexture();

	virtual Spectrum Evalute( const Vector2f& uv , const Point3f& p ) const;

public:
	void Deserialization( tinyxml2::XMLElement* RootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );

public:
	void GetValue( Spectrum& value1 , Spectrum& value2 );

	void SetValue( Spectrum& value , int index );

private:
	ConstantTexture* texture1;
	ConstantTexture* texture2;
};