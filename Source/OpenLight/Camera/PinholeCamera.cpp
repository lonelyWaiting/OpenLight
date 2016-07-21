#include "Utilities/PCH.h"
#include "Math/Vector3.h"
#include "Math/Point3.h"
#include "Math/Vector2.h"
#include "Core/Scene.h"
#include "Film/Film.h"
#include "Camera.h"
#include "tinyxml2.h"
#include "PinholeCamera.h"
#include "Utilities/srString.h"

PinholeCamera::PinholeCamera()
{
	ExposureTime = 0.0;

	fovy = 45.0;

	NearPlane = 0.0;
}

PinholeCamera::PinholeCamera( const PinholeCamera& rhs )
	:Camera( rhs )
{

}

Rayf PinholeCamera::GenerateRay( float RasterX , float RasterY , const CameraSample& SamplePoint )
{
	Vector2f RasterResolution = GetFilm()->GetResolution();

	// 视平面上的采样点
	float x = ( RasterX + SamplePoint.ImageSamples.x ) / RasterResolution.x  * ( Right - Left ) + Left;
	float y = ( 1.0 - ( RasterY + SamplePoint.ImageSamples.y ) / RasterResolution.y ) * ( Top - Bottom ) + Bottom;

	Vector3f dir = x * uvw.U + y * uvw.V + uvw.W;

	return Rayf( Eye + Normalize( dir ) * NearPlane , Normalize( dir ) , 1e-3f );
}

void PinholeCamera::Deserialization( tinyxml2::XMLElement* CameraRootElement )
{
	ParseVector( CameraRootElement->FirstChildElement( "Position" )->GetText() , &Eye[0] );

	ParseVector( CameraRootElement->FirstChildElement( "Target" )->GetText() , &Target[0] );

	tinyxml2::XMLElement* pElement = nullptr;

	pElement = CameraRootElement->FirstChildElement( "ExposureTime" );
	if( pElement )
	{
		pElement->QueryFloatText( &ExposureTime );
	}
	else
	{
		ExposureTime = 0.0;
	}
	
	pElement = CameraRootElement->FirstChildElement( "Fovy" );
	if( pElement )
	{
		pElement->QueryFloatText( &fovy );
	}
	else
	{
		fovy = 45.0;
	}
	

	pElement = CameraRootElement->FirstChildElement( "NearPlane" );
	if( pElement )
	{
		pElement->QueryFloatText( &NearPlane );
	}
	else
	{
		NearPlane = 0.0;
	}
	

	UpdateProperty();
}

void PinholeCamera::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	pRootElement->SetAttribute( "type" , GetName() );

	{
		char* pText = new char[50];
		sprintf( pText , "%f,%f,%f" , Eye.x , Eye.y , Eye.z );

		tinyxml2::XMLElement* pEyeElement = xmlDoc.NewElement( "Position" );

		pEyeElement->SetText( pText );

		pRootElement->InsertEndChild( pEyeElement );

		SAFE_DELETE( pText );
	}

	{
		char* pText = new char[50];
		sprintf( pText , "%f,%f,%f" , Target.x , Target.y , Target.z );

		tinyxml2::XMLElement* pTargetElement = xmlDoc.NewElement( "Target" );

		pTargetElement->SetText( pText );

		pRootElement->InsertEndChild( pTargetElement );

		SAFE_DELETE( pText );
	}

	{
		tinyxml2::XMLElement* pExposureTimeElement = xmlDoc.NewElement( "ExposureTime" );

		pExposureTimeElement->SetText( ExposureTime );

		pRootElement->InsertEndChild( pExposureTimeElement );
	}

	{
		tinyxml2::XMLElement* pFovyElement = xmlDoc.NewElement( "Fovy" );

		pFovyElement->SetText( fovy );

		pRootElement->InsertEndChild( pFovyElement );
	}

	{
		tinyxml2::XMLElement* pFirstElement = xmlDoc.FirstChildElement();

		tinyxml2::XMLElement* pFilmElement = xmlDoc.NewElement( "Film" );

		pFirstElement->InsertEndChild( pFilmElement );

		GetFilm()->Serialization( xmlDoc , pFilmElement );
	}

	{
		tinyxml2::XMLElement* pNearPlaneElement = xmlDoc.NewElement( "NearPlane" );

		pNearPlaneElement->SetText( NearPlane );

		pRootElement->InsertEndChild( pNearPlaneElement );
	}
}