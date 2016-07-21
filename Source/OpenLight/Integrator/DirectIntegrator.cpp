#include "DirectIntegrator.h"
#include "tinyxml2.h"
#include <BRDF/BSDF.h>
#include "Primitive/IntersectRecord.h"
#include "Math/Ray.h"
#include "Math/Vector3.h"
#include "Math/Point3.h"
#include "Core/Scene.h"
#include "Utilities/RNG.h"
#include "Light/Light.h"
#include "Integrator/IntegratorPCH.h"

void DirectIntegrator::Deserialization( tinyxml2::XMLElement * IntegratorRootElement )
{
	IntegratorRootElement->QueryIntAttribute( "MaxDepth" , &mMaxDepth );
}

void DirectIntegrator::Serialization( tinyxml2::XMLDocument & xmlDoc , tinyxml2::XMLElement * pRootElement )
{
	pRootElement->SetAttribute( "type" , GetName() );

	{
		pRootElement->SetAttribute( "MaxDepth" , mMaxDepth );
	}
}

Spectrum DirectIntegrator::Li( const Scene* scene , const Renderer* renderer , IntersectRecord* record , Rayf* ray , Accelerator* pAccelerator ) const
{
	Spectrum L( 0.0 );

	BSDF* bsdf = record->GetBSDF();

	Vector3f wo = -ray->Direction;
	Point3f HitPoint = record->HitPoint;
	Vector3f nomral = record->normal;

	// 自发光
	L += record->Le( wo );

	L += UniformSampleOneLight( scene , renderer , pAccelerator , bsdf , record->HitPoint , wo , record->normal );

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

		L += Li;
	}

	SAFE_DELETE( bsdf );

	return L;

}