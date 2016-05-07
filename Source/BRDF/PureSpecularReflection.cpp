#include "Utilities/PCH.h"
#include "Math/Normal.h"
#include "PureSpecularReflection.h"

PureSpecularReflection::PureSpecularReflection( const Spectrum& R )
	: BxDF( BxDFType( REFLECTION | SPECULAR ) )
	, R( R )
{

}

PureSpecularReflection::~PureSpecularReflection()
{

}

Spectrum PureSpecularReflection::f( const Vector3f& wo , const Vector3f& wi ) const
{
	return Spectrum( 0.0 );
}

Spectrum PureSpecularReflection::Sample_f( const Vector3f& wo , const Normal& n , Vector3f* wi , const Point2f& samplePoint , double* pdf , bool& bNoOccur ) const
{
	*wi = Normalize( 2 * Dot( wo , n ) / n.Length() * Normalize( n ) - wo );

	*pdf = 1.0;

	return R / AbsDot( *wi , n );
}

double PureSpecularReflection::PDF( const Vector3f& wi , const Vector3f& wo ) const
{
	// 除了反射方向其他方向的PDF全部为0
	return 0;
}