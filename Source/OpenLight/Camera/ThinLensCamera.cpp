#include "Utilities/PCH.h"
#include "Sampler/Sampling.h"
#include "tinyxml2.h"
#include "ThinLensCamera.h"
#include "Utilities/srString.h"

ThinLensCamera::ThinLensCamera()
{
	LensRadius = 1.0;

	ExposureTime = 0.0;

	fovy = 45.0;

	NearPlane = 0.0;
}

ThinLensCamera::ThinLensCamera( const ThinLensCamera& rhs )
	:Camera( rhs )
{
	LensRadius = rhs.LensRadius;
	LensFocus = rhs.LensFocus;
}

void ThinLensCamera::SetLensRadius( float _LensRadius )
{
	if( LensRadius != _LensRadius )
	{
		LensRadius = _LensRadius;
	}
}

void ThinLensCamera::SetLensFocus( float _LensFocus )
{
	if( LensFocus != _LensFocus )
	{
		LensFocus = _LensFocus;
	}
}

Rayf ThinLensCamera::GenerateRay( float RasterX , float RasterY , const CameraSample& SamplePoint )
{
	Vector2f RasterResolution = GetFilm()->GetResolution();

	// x , y为视平面上的采样点
	Point2f ImageSamples;
	ImageSamples.x = ( RasterX + SamplePoint.ImageSamples.x ) / RasterResolution.x * ( Right - Left ) + Left;
	ImageSamples.y = ( 1.0f - ( RasterY + SamplePoint.ImageSamples.y ) / RasterResolution.y ) * ( Top - Bottom ) + Bottom;

	float x = ImageSamples.x  * LensFocus;
	float y = ImageSamples.y  * LensFocus;

	Point2f LensSamples = MapUnitSquareToUnitDisk( SamplePoint.LensSamples );

	Point3f Orig = Eye + LensRadius * LensSamples.x * uvw.U + LensRadius * LensSamples.y * uvw.V;

	Vector3f dir = ( x - LensRadius * LensSamples.x ) * uvw.U + ( y - LensRadius * LensSamples.y ) * uvw.V + LensFocus * uvw.W;

	return Rayf( Orig + Normalize( dir ) * NearPlane , Normalize( dir ) , 1e-3f );
}

void ThinLensCamera::Deserialization( tinyxml2::XMLElement* CameraRootElement )
{
	tinyxml2::XMLElement* pElement = nullptr;

	ParseVector( CameraRootElement->FirstChildElement( "Position" )->GetText() , &Eye[0] );

	ParseVector( CameraRootElement->FirstChildElement( "Target" )->GetText() , &Target[0] );

	CameraRootElement->FirstChildElement( "LensFocus" )->QueryFloatText( &LensFocus );

	pElement = CameraRootElement->FirstChildElement( "LensRadius" );
	if( pElement )
	{
		pElement->QueryFloatText( &LensRadius );
	}
	else
	{
		LensRadius = 1.0;
	}
	

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
	

	if( CameraRootElement->FirstChildElement( "NearPlane" ) )
	{
		CameraRootElement->FirstChildElement( "NearPlane" )->QueryFloatText( &NearPlane );
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
		char* pText = new char[50];
		sprintf( pText , "%f,%f,%f" , Eye.x , Eye.y , Eye.z );

		tinyxml2::XMLElement* pEyeElement = xmlDoc.NewElement( "Position" );

		pEyeElement->SetText( pText );

		pRootElement->InsertEndChild( pEyeElement );

		SAFE_DELETE_ARRAY( pText );
	}

	{
		char* pText = new char[50];
		sprintf( pText , "%f,%f,%f" , Target.x , Target.y , Target.z );

		tinyxml2::XMLElement* pTargetElement = xmlDoc.NewElement( "Target" );

		pTargetElement->SetText( pText );

		pRootElement->InsertEndChild( pTargetElement );

		SAFE_DELETE_ARRAY( pText );
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