#include "Utilities/PCH.h"
#include "Spectrum/Spectrum.h"
#include "Math/Ray.h"
#include "Renderer/Renderer.h"
#include "Primitive/IntersectRecord.h"
#include "Accelerator/Accelerator.h"
#include "BRDF/BSDF.h"
#include "BRDF/BxDF.h"
#include "Utilities/RNG.h"
#include "COre/Scene.h"
#include "IntegratorPCH.h"

Spectrum SpecularReflect( const Ray& ray , const Scene* scene , const Renderer* renderer , IntersectRecord* record , Accelerator* pAccelerator , BSDF* pBSDF , bool& bNoOccur )
{
	// 着色点位置
	const Point3f& p = record->HitPoint;

	// 着色点法线
	const Normal& normal = Normalize( record->normal );

	double pdf;

	// 光线方向的反方向为出射方向
	Vector3f wo = -1.0f * ray.Direction;

	Vector3f wi;

	Point2f SamplePoint( RNG::Get().GetDouble(), RNG::Get().GetDouble() );

	Spectrum f = pBSDF->Sample_f( wo , normal , &wi , SamplePoint , &pdf , BxDFType( REFLECTION | SPECULAR ) , bNoOccur );

	Spectrum L = 0.0;

	if( pdf > 0.0 && !f.IsBlack() && AbsDot( wi , normal ) != 0.0 )
	{
		// 沿着法线方向的偏移
		Vector3f bias = Vector3f( wi.x * 1e-2f , wi.y * 1e-2f , wi.z * 1e-2f );

		// 将光线沿着法线方向偏移
		Ray r( record->HitPoint + bias , wi , ray , 1e-4f , Infinity );

		Spectrum Li = renderer->Li( scene , &r , record );

		L = f * Li * AbsDot( wi , normal ) / pdf;

		return L;
	}

	return Spectrum( 0.0 );
}

Spectrum SpecularTransmit( const Ray& ray , const Scene* scene , const Renderer* renderer , IntersectRecord* record , Accelerator* pAccelerator , BSDF* pBSDF , bool& bNoOccur )
{
	// 着色点位置
	const Point3f& p = record->HitPoint;

	// 着色点法线
	const Normal& normal = Normalize( record->normal );

	double pdf;

	Vector3f wo = -1.0f * ray.Direction;

	Vector3f wi;

	Point2f SamplePoint( RNG::Get().GetDouble() , RNG::Get().GetDouble() );

	Spectrum f = pBSDF->Sample_f( wo , normal , &wi , SamplePoint , &pdf , BxDFType( TRANSMISSION | SPECULAR ) , bNoOccur );

	Spectrum L = 0.0;

	if( pdf > 0.0 && !f.IsBlack() && AbsDot( wi , normal ) != 0.0 )
	{
		// 沿着法线方向的偏移量
		Vector3f bias = Vector3f( wi.x * 0.01 , wi.y * 0.01 , wi.z * 0.01 );

		// 将光线沿着法线方向偏移
		Ray r( record->HitPoint + bias , wi , ray , 1e-3 );

		Spectrum Li = renderer->Li( scene , &r , record );

		L = f * Li * AbsDot( wi , normal ) / pdf;
	}

	return L;
}

double PowerHeuristic( int nf , double fPdf , int ng , double gPdf )
{
	return ( nf * fPdf * nf * fPdf ) / ( nf * fPdf * nf * fPdf + ng * ng * gPdf * gPdf );
}

Spectrum UniformSampleOneLight( const Scene* scene , const Renderer* renderer , Accelerator* pAccelerator , BSDF* bsdf , const Point3f& point , const Vector3f& wo , const Normal& n )
{
	// 随机选择一个光源
	int LightCount = ( int )( scene->GetLights().size() );

	if( LightCount == 0 )
	{
		return Spectrum( 0.0 );
	}

	int LightIndex = std::min( ( int )std::floor( RNG::Get().GetDouble() * LightCount ) , LightCount - 1 );

	Light* light = scene->GetLight( LightIndex );

	return EstimateDirect( scene , renderer , pAccelerator , light , bsdf , point , wo , n , BxDFType( ALL_TYPE & ~SPECULAR ) );
}

Spectrum EstimateDirect( const Scene* scene , const Renderer* renderer , Accelerator* pAccelerator , Light* light , BSDF* bsdf , const Point3f& point , const Vector3f& wo , const Normal& n , BxDFType flags )
{
	// http://lonelywaiting.github.io/MIS/

	Spectrum Ld( 0.0 );
	Vector3f wi;
	// -----------------------------------------采样光源-----------------------------------------------------------
	VisibilityTester visibility;
	double LightPdf;
	Spectrum Li = light->Sample_L( point , &wi , LightSample( RNG::Get().GetDouble() , RNG::Get().GetDouble() ) , &LightPdf , &visibility );

	if( LightPdf > 0.0 && !Li.IsBlack() && visibility.Unoccluded( scene , pAccelerator ) )
	{
		// 计算BSDF的值
		Spectrum f = bsdf->f( wo , wi , n , flags );			// 无论需不需要采样BSDF,f都是需要计算的

																// 若对于该方向对而言，BSDF是非零值，且相互可见
		if( !f.IsBlack() )
		{
			if( light->IsDelta() )		// 对于Delta Light而言，只采样Light，而不采样BSDF
			{
				Ld += f * AbsDot( wi , n ) * Li / LightPdf;
			}
			else
			{
				// 根据Mulity Importance Sampling计算权重
				// 平衡BRDF和Light之间的PDF
				double BsdfPdf = bsdf->PDF( wo , wi , n , flags );
				double weight = PowerHeuristic( 1 , LightPdf , 1 , BsdfPdf );
				Ld += f * AbsDot( wi , n ) * Li * weight / LightPdf;
			}
		}
	}

	// ----------------------------------采样BSDF------------------------------------------
	if( !light->IsDelta() )
	{
		// 只有当light为非Delta distribution时，才会采样BSDF
		double BsdfPdf = 0.0;
		bool bNoOccur;
		BxDFType BrdfType;
		Spectrum f = bsdf->Sample_f( wo , n , &wi , Point2f( RNG::Get().GetDouble() , RNG::Get().GetDouble() ) , &BsdfPdf , flags , bNoOccur , &BrdfType );
		if( !f.IsBlack() && BsdfPdf > 0.0 )
		{
			double weight = 1.0;

			if( !( BrdfType & SPECULAR ) )
			{
				// 非镜面，需要该情况下light的pdf，从而计算权值
				LightPdf = light->PDF( point , wi );
				if( LightPdf == 0.0 )
				{
					// 光源沿着该方向没有任何贡献
					return Ld;
				}

				weight = PowerHeuristic( 1 , BsdfPdf , 1 , LightPdf );
			}

			Spectrum Li( 0.0 );
			IntersectRecord record;
			Ray r( point , wi );
			if( scene->Intersect( r , &record ) )
			{
				if( record.primitivePtr->GetAreaLight() == light )
				{
					Li = record.Le( -wi );
				}

				// 否则被其它图元挡住了
			}

			if( !Li.IsBlack() )
			{
				Ld += f * Li * AbsDot( wi , n ) * weight / BsdfPdf;
			}
		}
	}

	return Ld;
}