#include "PCH.h"
#include "Math/Normal.h"
#include "SpecularReflection.h"

SpecularReflection::SpecularReflection( const Spectrum& R , const Fresnel* fresnel )
	: BxDF( BxDFType( REFLECTION | SPECULAR ) )
	, R( R )
	, fresnel( fresnel )
{

}

SpecularReflection::~SpecularReflection()
{
	SAFE_DELETE( fresnel );
}

Spectrum SpecularReflection::f( const Vector3f& wo , const Vector3f& wi ) const
{
	return Spectrum( 0.0 );
}

Spectrum SpecularReflection::Sample_f( const Vector3f& wo , const Normal& n , Vector3f* wi , const Point2f& samplePoint , double* pdf ) const
{
	*wi = Normalize( 2 * Dot( wo , n ) / n.Length() * Normalize( n ) - wo );

	*pdf = 1.0;

	return fresnel->Evalute( wo * n ) * R / AbsDot( *wi , n );
}

double SpecularReflection::PDF( const Vector3f& wi , const Vector3f& wo ) const
{
	// 除了反射方向其他方向的PDF全部为0
	return 0;
}