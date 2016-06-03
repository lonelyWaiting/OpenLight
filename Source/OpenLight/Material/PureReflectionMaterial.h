#pragma once

#include "Material/Material.h"
#include "Texture/Texture.h"

namespace tinyxml2
{
	class XMLDocument;
	class XMLElement;
}

class PureReflectionMaterial : public Material
{
	DECLARE_DYNAMIC_CREATE_DERIVED( PureReflectionMaterial , Material )

public:
	PureReflectionMaterial();

	BSDF* GetBSDF( const Vector2f& uv , const Point3f& point , const Normal& normal ) const;

	PureReflectionMaterial( Spectrum R );

public:
	void Deserialization( tinyxml2::XMLElement* RootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );

public:
	Texture* Getkr();

private:
	Texture* Kr;
};