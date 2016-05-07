#include "Utilities/PCH.h"
#include "Sampler/Sampling.h"
#include "Math/Normal.h"
#include "BxDF.h"

BxDF::BxDF( BxDFType type )
	:type( type )
{

}

BxDF::~BxDF()
{

}

Spectrum BxDF::Sample_f( const Vector3f& wo , const Normal& n , Vector3f* wi , const Point2f& samplePoint , double* pdf , bool& bNoOccur) const
{
	// 生成半球采样方向
	*wi = CosineSampleHemisphere( samplePoint );

	// 由于wi->z总是为正
	// 所以当wo为负值时
	// 需要反转wi->z,从而保证位于同一个半球
	if( Dot( n , *wi ) < 0.0 )
	{
		*wi *= -1;
	}

	// 计算该方向对的pdf
	*pdf = PDF( *wi , wo );

	return f( wo , *wi );
}

// wi是位于物体的局部空间
// 因此法线为(0 , 0 , 1)
// 因此cos(theta) = (0 , 0 , 1) * wi = wi.z
double BxDF::PDF( const Vector3f& wi , const Vector3f& wo ) const
{
	return ( wi.z * wo.z > 0.0 ) ? AbsDot( wi , wi ) * INV_PI : 0.0f;
}

Spectrum BxDF::rho( const Normal& n , int nSamples , Point2f* Samples1 , Point2f* Samples2 ) const
{
	Spectrum r = 0;

	bool bNoOccur = false;

	for( int i = 0; i < nSamples; i++ )
	{
		Vector3f wo , wi;

		// 根据传入的二维采样点
		// 生成半球内的随机采样方向
		wo = UniformSampleHemisphere( Samples2[i] );

		double PdfOutput = INV_TWO_PI; 
		double PdfInput = 0.0f;

		Spectrum f = Sample_f( wo , n , &wi , Samples1[i] , &PdfInput , bNoOccur );

		if( PdfInput > 0.0f )
		{
			r += f * fabs( wo.z ) * fabs( wi.z ) / ( PdfInput * PdfOutput );
		}
	}

	return r / ( PI * nSamples );
}

// 给定出射方向，以及一组采样点
// 该组采样点用于生成一组入射方向
Spectrum BxDF::rho( const Vector3f& wo , const Normal& n , int nSamples , Point2f* samples ) const
{ 
	Spectrum r = 0;

	bool bNoOccur = false;

	for( int i = 0; i < nSamples; i++ )
	{
		Vector3f wi;
		double pdf = 0.0f;

		Spectrum f = Sample_f( wo , n , &wi , samples[i] , &pdf , bNoOccur );

		if( pdf > 0.0 )
		{
			r += f * fabs( wi.z ) / pdf;
		}
	}

	return r / double( nSamples );
}