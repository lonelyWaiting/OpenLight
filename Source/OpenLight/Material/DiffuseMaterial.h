#pragma once

#include "Material/Material.h"
#include "Texture/Texture.h"

namespace tinyxml2
{
	class XMLDocument;
	class XMLElement;
}

/*
	Format:
		<Material type="DiffuseMaterial">
			<kd type="ConstantTexture" value="1.0,1.0,1.0"/>
		</Material>
	or:
		<Material type="DiffuseMaterial">
			<kd type="CheckboardTexture" value1="1.0,1.0,1.0" value2="0.0,0.0,0.0" />
		</Materail>

	Materail节点中的type为material class name
	kd节点根据纹理类型填充相关数据
*/
class DiffuseMaterial : public Material
{
	DECLARE_DYNAMIC_CREATE_DERIVED( DiffuseMaterial , Material )

public:
	DiffuseMaterial();

	DiffuseMaterial( Texture* kd );

	void SetKd( Texture* kd );

	BSDF* GetBSDF( const Vector2f& uv , const Point3f& point , const Normal& normal ) const;

public:
	void Deserialization( tinyxml2::XMLElement* RootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );

private:
	Texture* Kd;
};