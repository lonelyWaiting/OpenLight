#include "PCH.h"
#include "Math/Vector3.h"
#include "Sampler/Sampling.h"
#include "Math/Ray.h"
#include "Math/Normal.h"
#include "PointLight.h"

PointLight::PointLight( const Transform& LightToWorld , const Spectrum & _intensity )
	:Light( LightToWorld )
{
	LightPosWorld = LightToWorld( Point3f( 0.0f , 0.0f , 0.0f ) );

	intensity = _intensity;
}

Spectrum PointLight::Sample_L( const Scene* scene , LightSample& lightSample , Ray* ray , Normal* NormalShading , float* pdf ) const
{
	*ray = Ray( LightPosWorld , UniformSampleSphere( lightSample.value[0] , lightSample.value[1] ) , 0.0f , Infinity );

	*NormalShading = Normal( ray->Direction.x , ray->Direction.y , ray->Direction.z );

	*pdf = UniformSpherePDF();

	return intensity;
}

Spectrum PointLight::Sample_L( const Point3f& p , Vector3f* wi , float* pdf ) const
{
	// 计算入射光线
	*wi = Normalize( LightPosWorld - p );

	*pdf = 1.0f;

	// 计算在p点处的强度
	return intensity / ( LightPosWorld - p ).Length();
}

Spectrum PointLight::Power( const Scene* scene ) const
{
	return 4.0f * PI * intensity;
}

float PointLight::PDF( const Point3f& p , const Vector3f& wi ) const
{
	return 0;
}