#pragma once

#include "Math/Point2.h"
#include "BxDF.h"

class Lambertian : public BxDF
{
public:
	Lambertian( const Spectrum& reflectance );

	Spectrum f( const Vector3f& wo , const Vector3f& wi ) const;

	Spectrum rho( const Vector3f& wo , int nSamples , Point2f* samples );

	Spectrum rho( int nSamples , Point2f* samples1 , Point2f* samples2 );

private:
	Spectrum R;		// ∑¥…‰¬ 
};