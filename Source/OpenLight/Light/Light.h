#pragma once

#include "Math/Transform.h"
#include "Spectrum/Spectrum.h"
#include "Core/VSerializableObject.h"
#include "VisibilityTester.h"
#include "Core/VCustomRTTI.h"

class Scene;

struct LightSample
{
	LightSample( double x , double y )
	{
		value[0] = x;
		value[1] = y;
	}

	double value[2];
};

class Light : public VSerializableObject
{
	DECLARE_DYNAMIC_CREATE_BASE( Light )

public:
	Light();

	virtual ~Light();

	Light( const Transform& _LightToWorld , int _nSamples = 1 );

	virtual Spectrum Sample_L( const Point3f& p , Vector3f* wi , LightSample& _lightSample , double* pdf , VisibilityTester* pVisibility ) const = 0;

	virtual Spectrum Power( const Scene* scene ) const = 0;

	virtual double PDF( const Point3f& p , const Vector3f& wi ) const = 0;

	virtual Spectrum Le( const Point3f& p , const Normal& n , const Vector3f& wo ) const;

	virtual Spectrum Sample_L( const Scene* scene , LightSample& _lightSample , Ray* ray , Normal* NormalShading , double* pdf ) const = 0;

protected:
	Transform LightToWorld;
	Transform WorldToLight;

public:
	int nSamples;
};