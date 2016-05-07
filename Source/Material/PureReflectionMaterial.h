#pragma once

#include "Material/Material.h"

namespace tinyxml2
{
	class XMLElement;
}

class PureReflectionMaterial : public Material
{
	DECLARE_DYNAMIC_CREATE_DERIVED( PureReflectionMaterial , Material )

public:
	PureReflectionMaterial();

	BSDF* GetBSDF( const Point3f& point , const Normal& normal ) const;

	PureReflectionMaterial( Spectrum R , double ior );

public:
	void Deserialization( tinyxml2::XMLElement* RootElement );

private:
	mutable Spectrum R;
	double ior;
};