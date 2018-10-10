#include "Utilities/PCH.h"
#include "BSDF.h"
#include "IO/Log.h"
#include "BxDFHelper.h"
#include "BxDF.h"
#include "Math/MathHelper.h"

BSDF::BSDF()
{
	nBxDF = 0;
}

BSDF::~BSDF()
{
	for( int i = 0; i < nBxDF; i++ )
	{
		SAFE_DELETE( bxdfs[i] );
	}
}

void BSDF::Add( BxDF* bxdf )
{
	Assert( nBxDF < nMaxBXDF );
	bxdfs[nBxDF++] = bxdf;
}

Spectrum BSDF::f( const Vector3f& wo , const Vector3f& wi , const Vector3f& n , const BxDFType type )
{
	bool IsReflect = Dot( wi , n ) * Dot( wo , n ) > 0.0 ? true : false;

	Spectrum f( 0.0 );

	for( int i = 0; i < nBxDF; i++ )
	{
		if( bxdfs[i]->IsMatch( type ) )
		{
			if( ( IsReflect && ( bxdfs[i]->type & REFLECTION ) ) || ( !IsReflect && ( bxdfs[i]->type & TRANSMISSION ) ) )
			{
				f += bxdfs[i]->f( wo , wi );
			}
		}
	}

	return f;
}

Spectrum BSDF::Sample_f( const Vector3f&wo , const Vector3f& n , Vector3f* wi , const Point2f& samplePoint , float* pdf , BxDFType type , bool& bNoOccur , BxDFType* SampledBxDFType ) const
{
	// ����ƥ���bxdf
	int MatchCount = Count( type );
	if( MatchCount == 0 )
	{
		*pdf = 0;

		if( SampledBxDFType != nullptr )
		{
			SampledBxDFType = nullptr;
		}

		return Spectrum( 0.0 );
	}

	// ͨ��������ȷ��ʹ�õڼ���ƥ���bxdf
	int index = srMath::Min( ( int )std::floor( samplePoint[0] * MatchCount ) , MatchCount - 1 );

	BxDF* bxdf = nullptr;
	for( int i = 0; i < nBxDF; i++ )
	{
		if(  bxdfs[i]->IsMatch( type ) && index-- == 0 )
		{
			bxdf = bxdfs[i];
			break;
		}
	}


	// ��ӳ�����ֵ��[0~1]^2
	Point2f RemappedSamplePoint( samplePoint[0] , samplePoint[1] );
	*pdf = 0.0;
	Spectrum f = bxdf->Sample_f( wo , n , wi , RemappedSamplePoint , pdf , bNoOccur );

	
	if( *pdf == 0 )
	{
		if( SampledBxDFType != nullptr )
		{
			SampledBxDFType = nullptr;
		}

		return 0;
	}

	if( SampledBxDFType != nullptr )
	{
		*SampledBxDFType = bxdf->type;
	}

	if( !( bxdf->type & SPECULAR ) && MatchCount > 1 )
	{
		for( int i = 0; i < nBxDF; i++ )
		{
			if( bxdfs[i] != bxdf && bxdfs[i]->IsMatch( type ) )
			{
				*pdf += bxdfs[i]->PDF( wo , *wi , n );
			}
		}
	}

	if( MatchCount > 1 )
	{
		*pdf /= MatchCount;
	}

	if( !( bxdf->type & SPECULAR ) && MatchCount > 1 )
	{
		bool IsReflect = Dot( wo , n ) * Dot( *wi , n ) > 0;
		f = 0.0;

		for( int i = 0; i < nBxDF; i++ )
		{
			if( bxdfs[i]->IsMatch( type ) )
			{
				if( ( IsReflect && ( bxdfs[i]->type & REFLECTION ) ) || ( !IsReflect && ( bxdfs[i]->type & TRANSMISSION ) ) )
				{
					f += bxdfs[i]->f( wo , *wi );
				}
			}
		}
	}

	return f;
}

float BSDF::PDF( const Vector3f& wo , const Vector3f& wi , const Vector3f& n , BxDFType type ) const
{
	float pdf = 0.0;
	int Num    = 0;

	// ������typeƥ���bxdf��pdf��
	for( int i = 0; i < nBxDF; i++ )
	{
		if( bxdfs[i]->IsMatch( type ) )
		{
			pdf += bxdfs[i]->PDF( wo , wi , n );
			Num++;
		}
	}

	return Num > 0 ? pdf / Num : 0.0f;
}

int BSDF::Count() const
{
	return nBxDF;
}

int BSDF::Count( BxDFType type ) const
{
	int count = 0;

	for( int i = 0; i < nBxDF; i++ )
	{
		if( bxdfs[i]->IsMatch( type ) )
		{
			count++;
		}
	}

	return count;
}