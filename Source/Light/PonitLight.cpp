#include "Utilities/PCH.h"
#include "Math/Vector3.h"
#include "Sampler/Sampling.h"
#include "Math/Ray.h"
#include "Math/Normal.h"
#include "tinyxml2.h"
#include "PointLight.h"

PointLight::PointLight()
{

}

PointLight::PointLight( const Transform& LightToWorld , const Spectrum & _intensity )
	:Light( LightToWorld )
{
	LightPosWorld = LightToWorld( Point3f( 0.0f , 0.0f , 0.0f ) );

	intensity = _intensity;
}

Spectrum PointLight::Sample_L( const Scene* scene , LightSample& lightSample , Ray* ray , Normal* NormalShading , double* pdf ) const
{
	*ray = Ray( LightPosWorld , UniformSampleSphere( lightSample.value[0] , lightSample.value[1] ) , 0.0f , Infinity );

	*NormalShading = Normal( ray->Direction.x , ray->Direction.y , ray->Direction.z );

	*pdf = UniformSpherePDF();

	return intensity;
}

Spectrum PointLight::Sample_L( const Point3f& p , Vector3f* wi , LightSample& _lightSample , double* pdf , VisibilityTester* pVisibility ) const
{
	// 计算入射光线
	*wi = Normalize( LightPosWorld - p );

	*pdf = 1.0f;
	
	// 创建线段
	pVisibility->SetSegment( p , EPSILON , LightPosWorld , 0 );

	// 计算在p点处的强度
	return intensity / ( LightPosWorld - p ).Length();
}

Spectrum PointLight::Power( const Scene* scene ) const
{
	return 4.0f * PI * intensity;
}

double PointLight::PDF( const Point3f& p , const Vector3f& wi ) const
{
	return 0;
}

void PointLight::Deserialization( tinyxml2::XMLElement* LightRootElement )
{
	tinyxml2::XMLElement* LightPositionRootElement = LightRootElement->FirstChildElement( "transform" )->FirstChildElement( "position" );
	LightPositionRootElement->FirstChildElement( "x" )->QueryDoubleText( &LightPosWorld.x );
	LightPositionRootElement->FirstChildElement( "y" )->QueryDoubleText( &LightPosWorld.y );
	LightPositionRootElement->FirstChildElement( "z" )->QueryDoubleText( &LightPosWorld.z );

	LightToWorld = Translate( Vector3f( LightPosWorld ) );
	WorldToLight = Inverse( LightToWorld );

	double r , g , b;
	tinyxml2::XMLElement* IntensityRootElement = LightRootElement->FirstChildElement( "intensity" );
	IntensityRootElement->FirstChildElement( "r" )->QueryDoubleText( &r );
	IntensityRootElement->FirstChildElement( "g" )->QueryDoubleText( &g );
	IntensityRootElement->FirstChildElement( "b" )->QueryDoubleText( &b );

	intensity = Spectrum::FromRGB( r , g , b );
}