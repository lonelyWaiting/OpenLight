#include "PCH.h"
#include "BRDF/BxDF.h"
#include "Primitive/IntersectRecord.h"
#include "Math/Normal.h"
#include "Math/Ray.h"
#include "Light/Light.h"
#include "Core/Scene.h"
#include "Light/VisibilityTester.h"
#include "Math/UtilitiesFunction.h"
#include "Renderer/Renderer.h"
#include "WhittedIntegrator.h"

Spectrum WhittedIntegrator::Li( const Scene* scene , const Renderer* renderer , IntersectRecord* record , Ray* ray ) const
{
	Spectrum L( 0.0 );

	// 计算Hit Point处的BRDF
	BxDF* brdf = record->GetBxDF();

	// 着色点位置，着色点法线
	const Point3f& p = record->HitPoint;

	const Normal& normal = record->normal;

	// 光线方向的反方向为出射方向
	Vector3f wo = -1.0f * ray->Direction;

	// 添加每个光源的贡献
	for( uint32_t i = 0; i < scene->GetLights().size(); i++ )
	{
		Vector3f wi;

		float pdf;

		VisibilityTester visibility;

		Spectrum Li = ( scene->GetLights() )[i]->Sample_L( p , &wi , &pdf );

		if( Li.IsBlack() || pdf == 0.0f )
		{
			continue;
		}

		Spectrum f = brdf->f( wo , wi );

		if( !f.IsBlack() && visibility.Unoccluded( scene ) )
		{
			L += f * Li * AbsDot( wi , normal ) / pdf;
		}
	}

	if( ray->depth + 1 < MaxDepth )
	{
		// 反射 ， 透射
		Ray reflectRay = Reflect( *ray , normal , record->HitPoint , 1e-3f );

		Spectrum Temp( 1.0f );
		L += renderer->Li( scene , &reflectRay , record , &Temp );
	}

	return L;
}

void WhittedIntegrator::SetMaxRecusiveDepth( int maxdepth )
{
	MaxDepth = maxdepth;
}