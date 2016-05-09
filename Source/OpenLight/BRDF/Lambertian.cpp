#include <Utilities/PCH.h>
#include "Lambertian.h"

Lambertian::Lambertian( const Spectrum& reflectance )
	: BxDF( BxDFType( DIFFUSE | REFLECTION ) )
	, R( reflectance )
{ 

}

Spectrum Lambertian::f( const Vector3f& wo , const Vector3f& wi ) const
{
	return R / PI;
}

Spectrum Lambertian::rho( const Normal& n , int nSamples , Point2f* samples1 , Point2f* samples2 )
{
	return R;
}

Spectrum Lambertian::rho( const Vector3f& wo , const Normal& n , int nSamples , Point2f* samples )
{
	return R;
}