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

bool PointLight::IsDelta()
{
	return true;
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

void PointLight::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	pRootElement->SetAttribute( "type" , GetName() );

	{
		tinyxml2::XMLElement* pTransformElement = xmlDoc.NewElement( "transform" );

		pRootElement->InsertEndChild( pTransformElement );

		tinyxml2::XMLElement* pPositionElement = xmlDoc.NewElement( "position" );

		pTransformElement->InsertEndChild( pPositionElement );

		tinyxml2::XMLElement* pXElement = xmlDoc.NewElement( "x" );

		pXElement->SetText( LightPosWorld.x );

		pPositionElement->InsertEndChild( pXElement );

		tinyxml2::XMLElement* pYElement = xmlDoc.NewElement( "y" );

		pYElement->SetText( LightPosWorld.y );

		pPositionElement->InsertEndChild( pYElement );

		tinyxml2::XMLElement* pZElement = xmlDoc.NewElement( "z" );

		pZElement->SetText( LightPosWorld.z );

		pPositionElement->InsertEndChild( pZElement );
	}

	{
		tinyxml2::XMLElement* pIntensityElement = xmlDoc.NewElement( "intensity" );

		pRootElement->InsertEndChild( pIntensityElement );

		tinyxml2::XMLElement* pRElement = xmlDoc.NewElement( "r" );

		pRElement->SetText( intensity[0] );

		pIntensityElement->InsertEndChild( pRElement );

		tinyxml2::XMLElement* pGElement = xmlDoc.NewElement( "g" );

		pGElement->SetText( intensity[1] );

		pIntensityElement->InsertEndChild( pGElement );

		tinyxml2::XMLElement* pBElement = xmlDoc.NewElement( "b" );

		pBElement->SetText( intensity[2] );

		pIntensityElement->InsertEndChild( pBElement );
	}
}