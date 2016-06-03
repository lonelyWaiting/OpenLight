#pragma once

#include "Texture.h"

/*
	Format:
		<kd type="ConstantTexture" value="1.0,1.0,1.0" />
	kd表示该纹理节点的名字，不一定为该名字
	在DiffuseMaterial Desserialization时，遇到kd节点，就当成纹理处理
	纹理节点不会单独出现，必定是捆绑在某个Material上，其名字在Material内部已经确定
*/
class ConstantTexture : public Texture
{
	DECLARE_DYNAMIC_CREATE_DERIVED( ConstantTexture , Texture )

public:
	ConstantTexture();

	ConstantTexture( const Spectrum& v );

	virtual Spectrum Evalute( const Vector2f& uv , const Point3f& p ) const;

	Spectrum& GetValue();

public:
	void Deserialization( tinyxml2::XMLElement* RootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );

private:
	Spectrum value;
};