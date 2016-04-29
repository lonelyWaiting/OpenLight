#include "PCH.h"
#include "Math/Normal.h"
#include "BxDFHelper.h"
#include "SpecularTransmission.h"

SpecularTransmission::SpecularTransmission( double etaI , double etaT , const Spectrum & T )
	: BxDF( BxDFType( TRANSMISSION | SPECULAR ) )
	, etaI( etaI )
	, etaT( etaT )
	, T( T )
	, fresnel( etaI , etaT )
{

}

Spectrum SpecularTransmission::f( const Vector3f& wo , const Vector3f& wi ) const
{
	return Spectrum( 0.0 );
}

Spectrum SpecularTransmission::Sample_f( const Vector3f& wo , const Normal& n , Vector3f* wi , const Point2f& samplePoint , double* pdf ) const
{
	// 计算折射方向
	bool entering = Dot( n , wo ) > 0.0;

	if( !entering )
	{
		Swap( etaI , etaT );
	}

	if( !Refract( wo , n , etaI / etaT , wi ) )
	{
		// 未发生折射
		return 0;
	}

	// 在折射方向上pdf位1
	*pdf = 1.0;

	return ( etaI * etaI ) / ( etaT * etaT ) * T * ( Spectrum( 1.0 ) - fresnel.Evalute( Dot( *wi , n ) ) ) / AbsDot( *wi , n );
}

double SpecularTransmission::PDF( const Vector3f& wi , const Vector3f& wo ) const
{
	return 0;
}