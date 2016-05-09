#include "Utilities/PCH.h"
#include "Math/Vector3.h"
#include "Math/Point3.h"
#include "Math/Vector2.h"
#include "Core/Scene.h"
#include "Film/Film.h"
#include "Camera.h"
#include "tinyxml2.h"
#include "PinholeCamera.h"

PinholeCamera::PinholeCamera()
{

}

PinholeCamera::PinholeCamera( const PinholeCamera& rhs )
	:Camera( rhs )
{

}

Ray PinholeCamera::GenerateRay( double RasterX , double RasterY , const CameraSample& SamplePoint )
{
	Vector2f RasterResolution = GetFilm()->GetResolution();

	// 视平面上的采样点
	double x = ( RasterX + SamplePoint.ImageSamples.x ) / RasterResolution.x  * ( Right - Left ) + Left;
	double y = ( 1.0 - ( RasterY + SamplePoint.ImageSamples.y ) / RasterResolution.y ) * ( Top - Bottom ) + Bottom;

	Vector3f dir = x * uvw.U + y * uvw.V + ViewDistance * uvw.W;

	return Ray( Eye , Normalize( dir ) , 1e-3f );
}

void PinholeCamera::Deserialization( tinyxml2::XMLElement* CameraRootElement )
{
	CameraRootElement->FirstChildElement( "Position" )->FirstChildElement( "x" )->QueryDoubleText( &( Eye.x ) );
	CameraRootElement->FirstChildElement( "Position" )->FirstChildElement( "y" )->QueryDoubleText( &( Eye.y ) );
	CameraRootElement->FirstChildElement( "Position" )->FirstChildElement( "z" )->QueryDoubleText( &( Eye.z ) );

	CameraRootElement->FirstChildElement( "Target" )->FirstChildElement( "x" )->QueryDoubleText( &( Target.x ) );
	CameraRootElement->FirstChildElement( "Target" )->FirstChildElement( "y" )->QueryDoubleText( &( Target.y ) );
	CameraRootElement->FirstChildElement( "Target" )->FirstChildElement( "z" )->QueryDoubleText( &( Target.z ) );

	CameraRootElement->FirstChildElement( "ExposureTime" )->QueryDoubleText( &ExposureTime );

	CameraRootElement->FirstChildElement( "ViewDistance" )->QueryDoubleText( &ViewDistance );
	
	CameraRootElement->FirstChildElement( "Fovy" )->QueryDoubleText( &fovy );

	UpdateProperty();
}

void PinholeCamera::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
{
	pRootElement->SetAttribute( "type" , GetName() );

	{
		tinyxml2::XMLElement* pEyeElement = xmlDoc.NewElement( "Position" );

		pRootElement->InsertEndChild( pEyeElement );

		tinyxml2::XMLElement* pXElement = xmlDoc.NewElement( "x" );

		pXElement->SetText( Eye.x );

		pEyeElement->InsertEndChild( pXElement );

		tinyxml2::XMLElement* pYElement = xmlDoc.NewElement( "y" );

		pYElement->SetText( Eye.y );

		pEyeElement->InsertEndChild( pYElement );

		tinyxml2::XMLElement* pZElement = xmlDoc.NewElement( "z" );

		pZElement->SetText( Eye.z );

		pEyeElement->InsertEndChild( pZElement );
	}

	{
		tinyxml2::XMLElement* pTargetElement = xmlDoc.NewElement( "Target" );

		pRootElement->InsertEndChild( pTargetElement );

		tinyxml2::XMLElement* pXElement = xmlDoc.NewElement( "x" );

		pXElement->SetText( Target.x );

		pTargetElement->InsertEndChild( pXElement );

		tinyxml2::XMLElement* pYElement = xmlDoc.NewElement( "y" );

		pYElement->SetText( Target.y );

		pTargetElement->InsertEndChild( pYElement );

		tinyxml2::XMLElement* pZElement = xmlDoc.NewElement( "z" );

		pZElement->SetText( Target.z );

		pTargetElement->InsertEndChild( pZElement );
	}

	{
		tinyxml2::XMLElement* pExposureTimeElement = xmlDoc.NewElement( "ExposureTime" );

		pExposureTimeElement->SetText( ExposureTime );

		pRootElement->InsertEndChild( pExposureTimeElement );
	}

	{
		tinyxml2::XMLElement* pViewDistanceElement = xmlDoc.NewElement( "ViewDistance" );

		pViewDistanceElement->SetText( ViewDistance );

		pRootElement->InsertEndChild( pViewDistanceElement );
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
}