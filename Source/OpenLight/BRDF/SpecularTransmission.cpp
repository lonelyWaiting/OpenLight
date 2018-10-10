#include "Utilities/PCH.h"
#include "BxDFHelper.h"
#include "SpecularTransmission.h"

SpecularTransmission::SpecularTransmission( float etaI , float etaT , const Spectrum & T )
	: BxDF( BxDFType( TRANSMISSION | SPECULAR ) )
	, etaI( etaI )
	, etaT( etaT )
	, T( T )
	, fresnel( etaI , etaT )
{

}

SpecularTransmission::~SpecularTransmission()
{

}

Spectrum SpecularTransmission::f( const Vector3f& wo , const Vector3f& wi ) const
{
	return Spectrum( 0.0 );
}

Spectrum SpecularTransmission::Sample_f( const Vector3f& wo , const Vector3f& n , Vector3f* wi , const Point2f& samplePoint , float* pdf , bool& bNoOccur ) const
{
	// 计算折射方向
	bool entering = Dot( n , wo ) > 0.0;

	float newetaI = etaI;
	float newetaT = etaT;

	if( !entering )
	{
		Swap( newetaI , newetaT );
	}

	if( !Refract( wo , n , newetaI / newetaT , wi , entering ) )
	{
		// 未发生折射
		return Spectrum( 0 );
	}

	// 在折射方向上pdf位1
	*pdf = 1.0;

	Spectrum F = fresnel.Evalute( Dot( wo , n ) );

	float P = 0.25f + 0.5f * F[0];

	return ( newetaT * newetaT ) / ( newetaI * newetaI ) * T * ( Spectrum( 1.0f ) - F ) / ( AbsDot( *wi , n ) * ( 1.0f - P ) );
}

float SpecularTransmission::PDF( const Vector3f& wi , const Vector3f& wo , const Vector3f& n ) const
{
	return 0;
}