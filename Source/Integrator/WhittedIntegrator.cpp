#include "PCH.h"
#include "BRDF/BxDF.h"
#include "Primitive/IntersectRecord.h"
#include "Math/Ray.h"
#include "BRDF/BSDF.h"
#include "Integrator/IntegratorPCH.h"
#include "WhittedIntegrator.h"

Spectrum WhittedIntegrator::Li( const Scene* scene , const Renderer* renderer , IntersectRecord* record , Ray* ray , Accelerator* pAccelerator ) const
{
	Spectrum L( 0 );

	// 计算Hit Point处的BRDF
	BSDF* bsdf = record->GetBSDF();

	// 着色点位置，着色点法线
	const Point3f& p = record->HitPoint;

	const Normal& normal = record->normal;

	// 光线方向的反方向为出射方向
	Vector3f wo = -1.0f * ray->Direction;

	// 自发光
	L += record->Emmisive;

	// 环境光 ， TO DO...

	// Ligtt , TO DO...

	if( ray->depth < mMaxDepth )
	{
		// 跟踪反射光线
		L += SpecularReflect( *ray , scene , renderer , record , pAccelerator , bsdf );

		// 跟踪折射光线
		L += SpecularTransmit( *ray , scene , renderer , record , pAccelerator , bsdf );
	}

	return L;
}

void WhittedIntegrator::SetMaxRecusiveDepth( int maxdepth )
{
	mMaxDepth = maxdepth;
}

void WhittedIntegrator::Deserialization( XMLElement* IntegratorRootElement )
{
	IntegratorRootElement->FirstChildElement( "MaxDepth" )->QueryIntText( &mMaxDepth );
}