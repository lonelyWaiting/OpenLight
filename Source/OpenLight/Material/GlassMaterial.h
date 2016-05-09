#pragma once

#include "Material/Material.h"

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

	BSDF* GetBSDF( const Point3f& point , const Normal& normal ) const;

	GlassMaterial( Spectrum R , Spectrum T , double ior );

public:
	void Deserialization( tinyxml2::XMLElement* RootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );

private:
	mutable Spectrum R;
	mutable Spectrum T;
	double ior;
};