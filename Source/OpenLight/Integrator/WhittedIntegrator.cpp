#include "Utilities/PCH.h"
#include "BRDF/BxDF.h"
#include "Primitive/IntersectRecord.h"
#include "Math/Ray.h"
#include "BRDF/BSDF.h"
#include "Integrator/IntegratorPCH.h"
#include "Core/Scene.h"
#include "Light/Light.h"
#include "WhittedIntegrator.h"
#include "tinyxml2.h"
#include "Utilities/RNG.h"
#include "BRDF/BxDFHelper.h"

Spectrum WhittedIntegrator::Li( const Scene* scene , const Renderer* renderer , IntersectRecord* record , Rayf* ray , Accelerator* pAccelerator ) const
{
	Spectrum L( 0.0 );

	// 计算Hit Point处的BRDF
	BSDF* bsdf = record->GetBSDF();

	Point3f HitPoint = record->HitPoint;
	Vector3f n = Normalize( record->normal );

	Vector3f wo = -ray->Direction;

	L += record->Le( wo );

	for( unsigned int i = 0; i < (unsigned int)scene->GetLightCount(); i++ )
	{
		Vector3f wi;
		float pdf;
		VisibilityTester* pVisibility = new VisibilityTester;

		Spectrum Li = scene->GetLight( i )->Sample_L( HitPoint , &wi , LightSample( RNG::Get().GetFloat() , RNG::Get().GetFloat() ) , &pdf , pVisibility );

		if( Li.IsBlack() || pdf == 0.0f )
		{
			SAFE_DELETE( pVisibility );
			continue;
		}

		Spectrum f = bsdf->f( wo , wi , n , BxDFType( DIFFUSE | SPECULAR | REFLECTION | TRANSMISSION ) );

		if( !f.IsBlack() && pVisibility->Unoccluded( scene , pAccelerator ) )
		{
			L += f * Li * AbsDot( wi , n ) / pdf;
		}

		SAFE_DELETE( pVisibility );
	}

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

void WhittedIntegrator::Deserialization( tinyxml2::XMLElement* IntegratorRootElement )
{
	IntegratorRootElement->QueryIntAttribute( "MaxDepth" , &mMaxDepth );
}

void WhittedIntegrator::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	pRootElement->SetAttribute( "type" , GetName() );

	{
		pRootElement->SetAttribute( "MaxDepth" , mMaxDepth );
	}
}