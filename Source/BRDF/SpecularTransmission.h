#pragma once

#include "BxDF.h"
#include "Fresnel.h"
#include "FresnelDielectric.h"

class SpecularTransmission : public BxDF
{
public:
	SpecularTransmission( double etaI , double etaT , const Spectrum & T );

	Spectrum f( const Vector3f& wo , const Vector3f& wi ) const;

	Spectrum Sample_f( const Vector3f& wo , const Normal& n , Vector3f* wi , const Point2f& samplePoint , double* pdf ) const;

	double PDF( const Vector3f& wi , const Vector3f& wo ) const;

private:
	double etaI;
	double etaT;
	const Spectrum	T;		// use to scale transmission color
	const FresnelDielectric fresnel;
};