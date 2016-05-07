#include "Utilities/PCH.h"
#include "IO/Log.h"
#include "BxDF.h"
#include "Math/Normal.h"
#include "BSDF.h"

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

Spectrum BSDF::f( const Vector3f& wo , const Vector3f& wi , const Normal& n , const BxDFType type )
{
	bool IsReflect = Dot( wi , n ) * Dot( wo , n ) > 0.0 ? true : false;

	Spectrum f( 0.0 );

	for( int i = 0; i < nBxDF; i++ )
	{
		if( ( bxdfs[i]->type & type ) == bxdfs[i]->type )
		{
			if( ( IsReflect && ( bxdfs[i]->type & REFLECTION ) ) || ( !IsReflect && ( bxdfs[i]->type & TRANSMISSION ) ) )
			{
				f += bxdfs[i]->f( wo , wi );
			}
		}
	}

	return f;
}

Spectrum BSDF::Sample_f( const Vector3f&wo , const Normal& n , Vector3f* wi , const Point2f& samplePoint , double* pdf , BxDFType type , bool& bNoOccur ) const
{
	// 计算匹配的bxdf
	int MatchCount = Count( type );
	if( MatchCount == 0 )
	{
		*pdf = 0;

		return Spectrum( 0.0 );
	}

	// 通过采样点确定使用第几个匹配的bxdf
	int index = std::min( ( int )std::floor( samplePoint[0] * MatchCount ) , MatchCount - 1 );

	BxDF* bxdf = nullptr;
	for( int i = 0; i < nBxDF; i++ )
	{
		if( ( bxdfs[i]->type & type ) == bxdfs[i]->type && index-- == 0 )
		{
			bxdf = bxdfs[i];
			break;
		}
	}


	// 重映射采样值到[0~1]^2
	Point2f RemappedSamplePoint( samplePoint[0] * MatchCount - index , samplePoint[1] );
	*pdf = 0.0;
	Spectrum f = bxdf->Sample_f( wo , n , wi , RemappedSamplePoint , pdf , bNoOccur );

	if( *pdf == 0 )
	{
		return 0;
	}

	if( !( bxdf->type & SPECULAR ) && MatchCount > 1 )
	{
		for( int i = 0; i < nBxDF; i++ )
		{
			if( bxdfs[i] != bxdf && ( bxdfs[i]->type & type ) == bxdfs[i]->type )
			{
				*pdf += bxdfs[i]->PDF( wo , *wi );
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
			if( ( bxdfs[i]->type & type ) == bxdfs[i]->type )
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

double BSDF::PDF( const Vector3f& wo , const Vector3f& wi , BxDFType type ) const
{
	double pdf = 0.0;
	int Num    = 0;

	// 计算与type匹配的bxdf的pdf和
	for( int i = 0; i < nBxDF; i++ )
	{
		if( ( bxdfs[i]->type & type ) == type )
		{
			pdf += bxdfs[i]->PDF( wo , wi );
			Num++;
		}
	}

	return Num > 0 ? pdf / Num : 0.0;
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
		if( ( bxdfs[i]->type & type ) == bxdfs[i]->type )
		{
			count++;
		}
	}

	return count;
}