#pragma once

#include "Material/Material.h"
#include "Texture/Texture.h"

namespace tinyxml2
{
	class XMLDocument;
	class XMLElement;
}

class GlassMaterial : public Material
{
	DECLARE_DYNAMIC_CREATE_DERIVED( GlassMaterial , Material )

public:
	GlassMaterial();

	BSDF* GetBSDF( const Vector2f& uv , const Point3f& point , const Vector3f& normal ) const;

	GlassMaterial( Spectrum R , Spectrum T , float ior );

public:
	void Deserialization( tinyxml2::XMLElement* RootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );

public:
	Texture* GetKr();
	Texture* GetKt();
	Texture* GetRefraIndex();

private:
	Texture* Kr;
	Texture* Kt;

	Texture* RefraIndex;		// ’€…‰±»
};