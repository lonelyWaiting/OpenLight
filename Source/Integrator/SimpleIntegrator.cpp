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
#include "Sampler/Sampling.h"
#include "Accelerator/Grid.h"
#include "SimpleIntegrator.h"

IMPLEMENT_DYNAMIC_CREATE_DERIVED( SimpleIntegrator , SurfaceIntegrator )

Spectrum SimpleIntegrator::Li( const Scene* scene , const Renderer* renderer , IntersectRecord* record , Ray* ray , Accelerator* pAccelerator ) const
{
	Spectrum L( 0.0 );

	if (ray->depth < MaxDepth)
	{
		// 计算Hit Point处的BRDF
		BSDF* brdf = record->GetBSDF();

		// 着色点位置，着色点法线
		const Point3f& p = record->HitPoint;

		const Normal& normal = record->normal;

		// 光线方向的反方向为出射方向
		Vector3f wo = -1.0f * ray->Direction;

		for (int i = 0; i < SamplePathNumber; i++)
		{
			double u0 = (double)rand() / (double)RAND_MAX;
			double u1 = (double)rand() / (double)RAND_MAX;

			// 采样半球
			Vector3f& wi = CosineSampleHemisphere(Point2f(u0, u1));

			if (Dot(normal, wi) < 0)
			{
				wi *= -1;
			}

			// 沿着法线方向的偏移量
			Vector3f bias = Vector3f(normal.x * EPSILON , normal.y * EPSILON , normal.z * EPSILON );

			// 将光线沿着法线方向偏移
			Ray r(record->HitPoint + bias , wi, *ray, 1e-3f , Infinity);

			// 若该入射方向与场景图元存在交点
			if( pAccelerator->Intersect( r , scene , record ) )
			{
				// pdf强行使用INV_PI
				L += Li( scene , renderer , record , &r , pAccelerator ) * brdf->f( wo , wi , record->normal , BxDFType( DIFFUSE ) ) *AbsDot( normal , wi ) / INV_PI;
			}
		}

		L /= SamplePathNumber;
	}
	
	L += record->Emission;

	return L;
}

void SimpleIntegrator::SetMaxRecusiveDepth( int maxdepth )
{
	MaxDepth = maxdepth;
}

void SimpleIntegrator::Deserialization(XMLElement* IntegratorRootElement)
{
	IntegratorRootElement->FirstChildElement("MaxDepth")->QueryIntText( &MaxDepth );

	IntegratorRootElement->FirstChildElement( "SamplePathNumber" )->QueryIntText( &SamplePathNumber );
}