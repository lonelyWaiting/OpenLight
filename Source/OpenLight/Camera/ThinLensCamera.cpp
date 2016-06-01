#include "Utilities/PCH.h"
#include "Sampler/Sampling.h"
#include "tinyxml2.h"
#include "ThinLensCamera.h"
#include "Utilities/srString.h"

ThinLensCamera::ThinLensCamera()
{

}

ThinLensCamera::ThinLensCamera( const ThinLensCamera& rhs )
	:Camera( rhs )
{
	LensRadius = rhs.LensRadius;
	LensFocus = rhs.LensFocus;
}

void ThinLensCamera::SetLensRadius( double _LensRadius )
{
	if( LensRadius != _LensRadius )
	{
		LensRadius = _LensRadius;
	}
}

void ThinLensCamera::SetLensFocus( double _LensFocus )
{
	if( LensFocus != _LensFocus )
	{
		LensFocus = _LensFocus;
	}
}

Ray ThinLensCamera::GenerateRay( double RasterX , double RasterY , const CameraSample& SamplePoint )
{
	Vector2f RasterResolution = GetFilm()->GetResolution();

	// x , y为视平面上的采样点
	Point2f ImageSamples;
	ImageSamples.x = ( RasterX + SamplePoint.ImageSamples.x ) / RasterResolution.x * ( Right - Left ) + Left;
	ImageSamples.y = ( 1.0f - ( RasterY + SamplePoint.ImageSamples.y ) / RasterResolution.y ) * ( Top - Bottom ) + Bottom;

	double x = ImageSamples.x / ViewDistance * LensFocus;
	double y = ImageSamples.y / ViewDistance * LensFocus;

	Point2f LensSamples = MapUnitSquareToUnitDisk( SamplePoint.LensSamples );

	Point3f Orig = Eye + LensRadius * LensSamples.x * uvw.U + LensRadius * LensSamples.y * uvw.V;

	Vector3f dir = ( x - LensRadius * LensSamples.x ) * uvw.U + ( y - LensRadius * LensSamples.y ) * uvw.V + LensFocus * uvw.W;

	return Ray( Orig + Normalize( dir ) * NearPlane , Normalize( dir ) , 1e-3 );
}

void ThinLensCamera::Deserialization( tinyxml2::XMLElement* CameraRootElement )
{
	tinyxml2::XMLElement* pElement = nullptr;

	ParseVector3( CameraRootElement->FirstChildElement( "Position" )->GetText() , &Eye[0] );

	ParseVector3( CameraRootElement->FirstChildElement( "Target" )->GetText() , &Target[0] );

	CameraRootElement->FirstChildElement( "LensFocus" )->QueryDoubleText( &LensFocus );

	pElement = CameraRootElement->FirstChildElement( "LensRadius" );
	if( pElement )
	{
		pElement->QueryDoubleText( &LensRadius );
	}
	else
	{
		LensRadius = 1.0;
	}
	

	pElement = CameraRootElement->FirstChildElement( "ExposureTime" );
	if( pElement )
	{
		pElement->QueryDoubleText( &ExposureTime );
	}
	else
	{
		ExposureTime = 0.0;
	}

	pElement = CameraRootElement->FirstChildElement( "ViewDistance" );
	if( pElement )
	{
		pElement->QueryDoubleText( &ViewDistance );
	}
	else
	{
		ViewDistance = 1.0;
	}
	

	pElement = CameraRootElement->FirstChildElement( "Fovy" );
	
	if( pElement )
	{
		pElement->QueryDoubleText( &fovy );
	}
	else
	{
		fovy = 45.0;
	}
	

	if( CameraRootElement->FirstChildElement( "NearPlane" ) )
	{
		CameraRootElement->FirstChildElement( "NearPlane" )->QueryDoubleText( &NearPlane );
	}
	else
	{
		NearPlane = 0.0;
	}
	

	UpdateProperty();
}

void ThinLensCamera::Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement )
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
		tinyxml2::XMLElement* pLensFocusTimeElement = xmlDoc.NewElement( "LensFocus" );

		pLensFocusTimeElement->SetText( LensFocus );

		pRootElement->InsertEndChild( pLensFocusTimeElement );
	}

	{
		tinyxml2::XMLElement* pLensRadiusTimeElement = xmlDoc.NewElement( "LensRadius" );

		pLensRadiusTimeElement->SetText( LensRadius );

		pRootElement->InsertEndChild( pLensRadiusTimeElement );
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
		tinyxml2::XMLElement* pNearPlaneElement = xmlDoc.NewElement( "NearPlane" );

		pNearPlaneElement->SetText( NearPlane );

		pRootElement->InsertEndChild( pNearPlaneElement );
	}

	{
		tinyxml2::XMLElement* pFirstElement = xmlDoc.FirstChildElement();

		tinyxml2::XMLElement* pFilmElement = xmlDoc.NewElement( "Film" );

		pFirstElement->InsertEndChild( pFilmElement );

		GetFilm()->Serialization( xmlDoc , pFilmElement );
	}
}