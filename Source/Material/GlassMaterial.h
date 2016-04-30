#pragma once

#include "Material/Material.h"

class GlassMaterial : public Material
{
	DECLARE_DYNAMIC_CREATE_DERIVED( GlassMaterial , Material )

public:
	GlassMaterial();

	BSDF* GetBSDF( const Point3f& point , const Normal& normal ) const;

	GlassMaterial( Spectrum R , Spectrum T , double ior );

public:
	void Deserialization( XMLElement* RootElement );

private:
	mutable Spectrum R;
	mutable Spectrum T;
	double ior;
};