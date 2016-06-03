#include "Utilities/PCH.h"
#include "Math/Vector3.h"
#include "Sampler/Sampling.h"
#include "Math/Ray.h"
#include "Math/Normal.h"
#include "tinyxml2.h"
#include "PointLight.h"
#include "Utilities/srString.h"

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
	tinyxml2::XMLElement* LightTransformRootElement = LightRootElement->FirstChildElement( "transform" );

	ParseVector( LightTransformRootElement->Attribute( "position" ) , &LightPosWorld[0] );

	LightToWorld = Translate( Vector3f( LightPosWorld ) );
	WorldToLight = Inverse( LightToWorld );

	double r , g , b;
	tinyxml2::XMLElement* IntensityRootElement = LightRootElement->FirstChildElement( "intensity" );

	ParseVector( LightRootElement->GetText() , intensity.GetDataPtr() );
}

void PointLight::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	pRootElement->SetAttribute( "type" , GetName() );

	{
		char* pText = new char[50];
		sprintf( pText , "%f,%f,%f" , LightPosWorld.x , LightPosWorld.y , LightPosWorld.z );

		tinyxml2::XMLElement* pTransformElement = xmlDoc.NewElement( "transform" );

		pTransformElement->SetAttribute( "position" , pText );

		pRootElement->InsertEndChild( pTransformElement );

		SAFE_DELETE( pText );
	}

	{
		char* pText = new char[50];
		sprintf( pText , "%f,%f,%f" , intensity[0] , intensity[1] , intensity[2] );

		tinyxml2::XMLElement* pIntensityElement = xmlDoc.NewElement( "intensity" );
		
		pIntensityElement->SetText( pText );

		pRootElement->InsertEndChild( pIntensityElement );
	}
}