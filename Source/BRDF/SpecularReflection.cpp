#include "PCH.h"
#include "Math/Normal.h"
#include "SpecularReflection.h"

SpecularReflection::SpecularReflection( const Spectrum& R , const Fresnel* fresnel )
	: R( R )
	, fresnel( fresnel )
{

}

Spectrum SpecularReflection::f( const Vector3f& wo , const Vector3f& wi ) const
{
	return Spectrum( 0.0 );
}

Spectrum SpecularReflection::Sample_f( const Vector3f& wo , const Normal& n , Vector3f* wi , const Point2f& samplePoint , double* pdf ) const
{
	*wi = Normalize( 2 * Dot( wo , n ) / n.Length() * n - wo );

	*pdf = 1.0;

	return fresnel->Evalute( *wi * n ) * R / Dot( *wi , n );
}

double SpecularReflection::PDF( const Vector3f& wi , const Vector3f& wo ) const
{
	// 除了反射方向其他方向的PDF全部为0
	return 0;
}