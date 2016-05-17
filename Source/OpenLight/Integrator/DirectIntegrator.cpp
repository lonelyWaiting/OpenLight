#include "DirectIntegrator.h"
#include "tinyxml2.h"
#include "BRDF/BSDF.h"
#include "Primitive/IntersectRecord.h"
#include "Math/Ray.h"
#include "Math/Vector3.h"
#include "Math/Point3.h"
#include "Math/Normal.h"
#include "Core/Scene.h"
#include "Utilities/RNG.h"
#include "Light/Light.h"
#include "Integrator/IntegratorPCH.h"

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
		Spectrum f = bsdf->f( wo , wi , n , flags );

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
				double BsdfPdf = bsdf->PDF( wo , wi , n ,  flags );
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
			else
			{
				Li = Spectrum( 0.0 );
			}

			if( !Li.IsBlack() )
			{
				Ld += f * Li * AbsDot( wi , n ) * weight / BsdfPdf;
			}
		}
	}

	return Ld;
}

void DirectIntegrator::Deserialization( tinyxml2::XMLElement * IntegratorRootElement )
{
	IntegratorRootElement->FirstChildElement( "MaxDepth" )->QueryIntText( &mMaxDepth );
}

void DirectIntegrator::Serialization( tinyxml2::XMLDocument & xmlDoc , tinyxml2::XMLElement * pRootElement )
{
	pRootElement->SetAttribute( "type" , GetName() );

	{
		tinyxml2::XMLElement* pMaxDepthElement = xmlDoc.NewElement( "MaxDepth" );

		pMaxDepthElement->SetText( mMaxDepth );

		pRootElement->InsertEndChild( pMaxDepthElement );
	}
}

Spectrum DirectIntegrator::Li( const Scene* scene , const Renderer* renderer , IntersectRecord* record , Ray* ray , Accelerator* pAccelerator ) const
{
	Spectrum L( 0.0 );

	BSDF* bsdf = record->GetBSDF();

	Vector3f wo = -ray->Direction;
	Point3f HitPoint = record->HitPoint;
	Normal nomral = record->normal;

	// 自发光
	L += record->Le( wo );

	L += UniformSampleOneLight( scene , renderer , pAccelerator , bsdf , record->HitPoint , wo , record->normal );

	L *= record->SurfaceColor;

	if( ray->depth < mMaxDepth )
	{
		Spectrum Li( 0.0 );

		bool bNoReflectOccur = false;

		// 跟踪反射光线
		Li += SpecularReflect( *ray , scene , renderer , record , pAccelerator , bsdf , bNoReflectOccur );

		if( bNoReflectOccur )
		{
			// 跟踪折射光线
			Li += SpecularTransmit( *ray , scene , renderer , record , pAccelerator , bsdf , bNoReflectOccur );
		}

		L = L + Li;
	}

	SAFE_DELETE( bsdf );

	return L;

}

