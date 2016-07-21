#pragma once

#include "BxDF.h"
#include "Fresnel.h"
#include "FresnelDielectric.h"

class SpecularTransmission : public BxDF
{
public:
	SpecularTransmission( float etaI , float etaT , const Spectrum & T );

	~SpecularTransmission();

	Spectrum f( const Vector3f& wo , const Vector3f& wi ) const;

	Spectrum Sample_f( const Vector3f& wo , const Vector3f& n , Vector3f* wi , const Point2f& samplePoint , float* pdf , bool& bNoOccur ) const;

	float PDF( const Vector3f& wi , const Vector3f& wo , const Vector3f& n ) const;

private:
	mutable float etaI;
	mutable float etaT;
	const Spectrum	T;		// use to scale transmission color
	const FresnelDielectric fresnel;
};