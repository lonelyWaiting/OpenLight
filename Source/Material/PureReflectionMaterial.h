#pragma once

#include "Material/Material.h"

class PureReflectionMaterial : public Material
{
	DECLARE_DYNAMIC_CREATE_DERIVED( PureReflectionMaterial , Material )

public:
	PureReflectionMaterial();

	BSDF* GetBSDF( const Point3f& point , const Normal& normal ) const;

	PureReflectionMaterial( Spectrum R , double ior );

public:
	void Deserialization( XMLElement* RootElement );

private:
	mutable Spectrum R;
	double ior;
};