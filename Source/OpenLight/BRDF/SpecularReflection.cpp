#include "Utilities/PCH.h"
#include "SpecularReflection.h"
#include "Utilities/RNG.h"

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

Spectrum SpecularReflection::Sample_f( const Vector3f& wo , const Vector3f& n , Vector3f* wi , const Point2f& samplePoint , float* pdf , bool& bNoOccur ) const
{
	*wi = Normalize( 2 * Dot( wo , n ) / n.Length() * Normalize( n ) - wo );

	*pdf = 1.0;

	Spectrum F = fresnel->Evalute( wo * n );

	// 经验设定
	// 参考该问题的答案：http://www.opengpu.org/forum.php?mod=viewthread&tid=18099&extra=page%3D1
	float P = 0.25f + 0.5f * F[0];

	if( RNG::Get().GetFloat() > /*F[0]*/P )
	{
		// 无反射
		bNoOccur = true;
		return Spectrum( 0 );
	}

	return F / P * R / AbsDot( *wi , n );
}

float SpecularReflection::PDF( const Vector3f& wi , const Vector3f& wo , const Vector3f& n ) const
{
	// 除了反射方向其他方向的PDF全部为0
	return 0;
}