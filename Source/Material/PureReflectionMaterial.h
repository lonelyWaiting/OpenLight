#pragma once

#include "Material/Material.h"

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

	BSDF* GetBSDF( const Point3f& point , const Normal& normal ) const;

	PureReflectionMaterial( Spectrum R );

public:
	void Deserialization( tinyxml2::XMLElement* RootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );

private:
	mutable Spectrum R;
};