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

Spectrum PathIntegrator::Li( const Scene* scene , const Renderer* renderer , IntersectRecord* record , Rayf* ray , Accelerator* pAccelerator ) const
{
	Spectrum L( 0.0 );
	Spectrum Throughout = 1.0;

	bool IsSpecular = false;
	int count = 0;

	IntersectRecord TempRecord;

	Rayf r;

	while( true )
	{
		Vector3f wo = -ray->Direction;

		if( count == 0 || IsSpecular )
		{
			// ×Ô·¢¹â
			L += Throughout * record->Le( wo );
		}

		BSDF* bsdf = record->GetBSDF();

		const Point3f& HitPoint = record->HitPoint;

		const Vector3f& HitNormal = Normalize( record->normal );

		L += Throughout * UniformSampleOneLight( scene , renderer , pAccelerator , bsdf , HitPoint , wo , HitNormal );

		Vector3f wi;
		float pdf = 0.0;
		BxDFType flags;
		
		bool bReject = false;

		Point2f BRDFSamplePoint = Point2f( RNG::Get().GetFloat() , RNG::Get().GetFloat() );

		Spectrum f = 0.0;

		if( bsdf->Count( BxDFType( SPECULAR | REFLECTION ) ) && bsdf->Count( BxDFType( SPECULAR | TRANSMISSION ) ) )
		{
			f = bsdf->Sample_f( wo , HitNormal , &wi , BRDFSamplePoint , &pdf , BxDFType( SPECULAR | REFLECTION ) , bReject , &flags );

			if( bReject )
			{
				f = bsdf->Sample_f( wo , HitNormal , &wi , BRDFSamplePoint , &pdf , BxDFType( SPECULAR | TRANSMISSION ) , bReject , &flags );
			}
		}
		else
		{
			f = bsdf->Sample_f( wo , HitNormal , &wi , BRDFSamplePoint , &pdf , BxDFType( ALL_TYPE ) , bReject , &flags );
		}

		SAFE_DELETE( bsdf );

		if( f.IsBlack() || pdf == 0.0 )
		{
			break;
		}

		IsSpecular = ( ( flags & SPECULAR ) != 0 );

		Throughout *= f * AbsDot( wi , HitNormal ) / pdf;

		r = Rayf( HitPoint + wi * 1e-3f , wi , *ray , EPSILON );

		ray = &r;

		if( count > 3 )
		{

		}

		if( count++ >= mMaxDepth )
		{
			break;
		}

		if( !scene->Intersect( *ray , record ) )
		{
			L += Throughout * scene->GetEnvironmentPtr()->Evalute( *ray );

			break;
		}
	}

	return L;
}

void PathIntegrator::Deserialization( tinyxml2::XMLElement * IntegratorRootElement )
{
	IntegratorRootElement->QueryIntAttribute( "MaxDepth" , &mMaxDepth );
}

void PathIntegrator::Serialization( tinyxml2::XMLDocument & xmlDoc , tinyxml2::XMLElement * pRootElement )
{
	pRootElement->SetAttribute( "type" , GetName() );

	{
		pRootElement->SetAttribute( "MaxDepth" , mMaxDepth );
	}
}
