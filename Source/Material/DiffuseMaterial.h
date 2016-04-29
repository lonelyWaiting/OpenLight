#pragma once

#include "Material/Material.h"

class DiffuseMaterial : public Material
{
public:
	DiffuseMaterial();

	BSDF* GetBSDF( const Point3f& point , const Normal& normal ) const;

	DiffuseMaterial( Spectrum R );

public:
	void Deserialization( XMLElement* RootElement );

private:
	mutable Spectrum R;
};