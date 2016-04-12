#pragma once

#include "Light.h"
#include "Math/Point3.h"

class PointLight : public Light
{
public:
	PointLight( const Transform& LightToWorld , const Spectrum & intensity );

	Spectrum Sample_L( const Point3f& p , Vector3f* wi , float* pdf ) const;

	Spectrum Power( const Scene* scene ) const;
	
	Spectrum Sample_L( const Scene* scene , LightSample& _lightSample , Ray* ray , Normal* NormalShading , float* pdf ) const;

	float PDF( const Point3f& p , const Vector3f& wi ) const;

private:
	Point3f LightPosWorld;
	Spectrum intensity;
};