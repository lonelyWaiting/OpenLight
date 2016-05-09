#include "Utilities/PCH.h"
#include "Spectrum/Spectrum.h"
#include "Math/Ray.h"
#include "Renderer/Renderer.h"
#include "Primitive/IntersectRecord.h"
#include "Accelerator/Accelerator.h"
#include "BRDF/BSDF.h"
#include "BRDF/BxDF.h"
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

	Point2f SamplePoint( ( double )rand() / ( double )RAND_MAX , ( double )rand() / ( double )RAND_MAX );

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

	Vector3f wi;

	Point2f SamplePoint( ( double )rand() / ( double )RAND_MAX , ( double )rand() / ( double )RAND_MAX );

	Spectrum f = pBSDF->Sample_f( ray.Direction , normal , &wi , SamplePoint , &pdf , BxDFType( TRANSMISSION | SPECULAR ) , bNoOccur );

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