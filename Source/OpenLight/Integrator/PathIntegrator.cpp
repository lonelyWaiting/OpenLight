#include "PathIntegrator.h"
#include "Spectrum/Spectrum.h"
#include "tinyxml2.h"
#include "BRDF/BxDFHelper.h"
#include "BRDF/BxDF.h"
#include "Primitive/IntersectRecord.h"
#include "Math/Ray.h"
#include "BRDF/BSDF.h"
#include "Integrator/IntegratorPCH.h"
#include "Core/Scene.h"
#include "Utilities/RNG.h"
#include "Light/Light.h"

Spectrum PathIntegrator::Li( const Scene* scene , const Renderer* renderer , IntersectRecord* record , Ray* ray , Accelerator* pAccelerator ) const
{
	Spectrum L( 0.0 );
	Spectrum Throughout = 1.0;

	bool IsSpecular = false;
	int count = 0;

	IntersectRecord TempRecord;

	Ray r;

	while( true )
	{
		Vector3f wo = -ray->Direction;

		if( count == 0 || IsSpecular )
		{
			// 自发光
			L += Throughout * record->Le( wo );
		}

		BSDF* bsdf = record->GetBSDF();

		const Point3f& HitPoint = record->HitPoint;

		const Normal& HitNormal = record->normal;

		L += Throughout * UniformSampleOneLight( scene , renderer , pAccelerator , bsdf , HitPoint , wo , HitNormal ) * record->SurfaceColor;

		Vector3f wi;
		double pdf = 0.0;
		BxDFType flags;
		
		bool bReject = false;

		Point2f BRDFSamplePoint = Point2f( RNG::Get().GetDouble() , RNG::Get().GetDouble() );

		// 计算wi和pdf
		Spectrum f = bsdf->Sample_f( wo , HitNormal , &wi , BRDFSamplePoint , &pdf , BxDFType( ALL_TYPE ) , bReject , &flags );

		if( bReject )
		{
			f = bsdf->Sample_f( wo , HitNormal , &wi , BRDFSamplePoint , &pdf , BxDFType( SPECULAR | TRANSMISSION ) , bReject , &flags );
		}

		SAFE_DELETE( bsdf );

		if( f.IsBlack() || pdf == 0.0 )
		{
			break;
		}

		IsSpecular = ( flags & SPECULAR ) != 0;

		Throughout *= f * AbsDot( wi , HitNormal ) / pdf * record->SurfaceColor;

		r = Ray( HitPoint + wi * 1e-3f , wi , *ray , EPSILON );

		ray = &r;

		if( count > 3 )
		{
			double continueProbability = Throughout.y() > 0.5 ? Throughout.y() : 0.5;

			if( RNG::Get().GetDouble() > continueProbability )
			{
				break;
			}

			Throughout /= continueProbability;
		}

		if( count++ >= mMaxDepth )
		{
			break;
		}

		if( !scene->Intersect( *ray , &TempRecord ) )
		{
			/*if( IsSpecular )
			{
				for( int i = 0; i < scene->GetLights().size(); i++ )
				{
					L += Throughout * scene->GetLight( i )->Le( HitPoint , HitNormal , -ray->Direction ) * record->SurfaceColor;
				}
			}*/

			break;
		}

		record = &TempRecord;
	}

	return L;
}

void PathIntegrator::Deserialization( tinyxml2::XMLElement * IntegratorRootElement )
{
	IntegratorRootElement->FirstChildElement( "MaxDepth" )->QueryIntText( &mMaxDepth );
}

void PathIntegrator::Serialization( tinyxml2::XMLDocument & xmlDoc , tinyxml2::XMLElement * pRootElement )
{
	pRootElement->SetAttribute( "type" , GetName() );

	{
		tinyxml2::XMLElement* pMaxDepthElement = xmlDoc.NewElement( "MaxDepth" );

		pMaxDepthElement->SetText( mMaxDepth );

		pRootElement->InsertEndChild( pMaxDepthElement );
	}
}
