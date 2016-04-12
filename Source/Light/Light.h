#pragma once

#include "Math/Transform.h"
#include "Spectrum/Spectrum.h"

class Scene;

struct LightSample
{
	float value[2];
};

class Light
{
public:
	virtual ~Light();

	Light( const Transform& _LightToWorld , int _nSamples = 1 );

	virtual Spectrum Sample_L( const Point3f& p , Vector3f* wi , float* pdf ) const = 0;

	virtual Spectrum Power( const Scene* scene ) const = 0;

	virtual float PDF( const Point3f& p , const Vector3f& wi ) const = 0;

	virtual Spectrum Le( const Ray& r ) const;

	virtual Spectrum Sample_L( const Scene* scene , LightSample& _lightSample , Ray* ray , Normal* NormalShading , float* pdf ) const = 0;

protected:
	const Transform LightToWorld;
	const Transform WorldToLight;

public:
	int nSamples;
};