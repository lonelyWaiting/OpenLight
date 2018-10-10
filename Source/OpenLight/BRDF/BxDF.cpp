#include "Utilities/PCH.h"
#include "Sampler/Sampling.h"
#include "BxDF.h"

BxDF::BxDF( BxDFType type )
	:type( type )
{

}

BxDF::~BxDF()
{

}

Spectrum BxDF::Sample_f( const Vector3f& wo , const Vector3f& n , Vector3f* wi , const Point2f& samplePoint , float* pdf , bool& bNoOccur) const
{
	// ���ɰ����������
	*wi = CosineSampleHemisphere( samplePoint );

	*wi = Normalize( *wi );

	// ����wi->z����Ϊ��
	// ���Ե�woΪ��ֵʱ
	// ��Ҫ��תwi->z,�Ӷ���֤λ��ͬһ������
	if( Dot( n , *wi ) * Dot( n , wo ) < 0.0 )
	{
		*wi *= -1;
	}

	// ����÷���Ե�pdf
	*pdf = PDF( *wi , wo , n );

	return f( wo , *wi );
}

// wi��λ������ľֲ��ռ�
// ��˷���Ϊ(0 , 0 , 1)
// ���cos(theta) = (0 , 0 , 1) * wi = wi.z
float BxDF::PDF( const Vector3f& wi , const Vector3f& wo , const Vector3f& n ) const
{
	return ( Dot( wi , n ) * Dot( wo , n ) > 0.0f ) ? AbsDot( wi , n ) * INV_PI : 0.0f;
}

bool BxDF::IsMatch( const BxDFType & flag )
{
	if( ( type & flag ) == type )
	{
		return true;
	}

	return false;
}

Spectrum BxDF::rho( const Vector3f& n , int nSamples , Point2f* Samples1 , Point2f* Samples2 ) const
{
	Spectrum r = 0;

	bool bNoOccur = false;

	for( int i = 0; i < nSamples; i++ )
	{
		Vector3f wo , wi;

		// ���ݴ���Ķ�ά������
		// ���ɰ����ڵ������������
		wo = UniformSampleHemisphere( Samples2[i] );

		float PdfOutput = INV_TWO_PI;
		float PdfInput = 0.0f;

		Spectrum f = Sample_f( wo , n , &wi , Samples1[i] , &PdfInput , bNoOccur );

		if( PdfInput > 0.0f )
		{
			r += f * fabs( wo.z ) * fabs( wi.z ) / ( PdfInput * PdfOutput );
		}
	}

	return r / ( PI * nSamples );
}

// �������䷽���Լ�һ�������
// �����������������һ�����䷽��
Spectrum BxDF::rho( const Vector3f& wo , const Vector3f& n , int nSamples , Point2f* samples ) const
{ 
	Spectrum r = 0;

	bool bNoOccur = false;

	for( int i = 0; i < nSamples; i++ )
	{
		Vector3f wi;
		float pdf = 0.0f;

		Spectrum f = Sample_f( wo , n , &wi , samples[i] , &pdf , bNoOccur );

		if( pdf > 0.0 )
		{
			r += f * fabs( wi.z ) / pdf;
		}
	}

	return r / float( nSamples );
}