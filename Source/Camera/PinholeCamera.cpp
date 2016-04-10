#include "PCH.h"
#include "Math/Vector3.h"
#include "Math/Point3.h"
#include "Math/Vector2.h"
#include "Core/Scene.h"
#include "Film/Film.h"
#include "Camera.h"
#include "PinholeCamera.h"

PinholeCamera::PinholeCamera()
{

}

PinholeCamera::PinholeCamera( const PinholeCamera& rhs )
	:Camera( rhs )
{

}

Ray PinholeCamera::GenerateRay( float RasterX , float RasterY , const CameraSample& SamplePoint )
{
	Vector2f RasterResolution = GetFilm()->GetResolution();

	// 视平面上的采样点
	float x = ( RasterX + SamplePoint.ImageSamples.x ) / RasterResolution.x  * ( Right - Left ) + Left;
	float y = ( RasterY + SamplePoint.ImageSamples.y ) / RasterResolution.y * ( Top - Bottom ) + Bottom;

	Vector3f dir = x * uvw.U + y * uvw.V + ViewDistance * uvw.W;

	return Ray( Eye , Normalize( dir ) , 1e-3f );
}

void PinholeCamera::ParseCamera( XMLElement* CameraRootElement )
{
	CameraRootElement->FirstChildElement( "Position" )->FirstChildElement( "x" )->QueryFloatText( &( Eye.x ) );
	CameraRootElement->FirstChildElement( "Position" )->FirstChildElement( "y" )->QueryFloatText( &( Eye.y ) );
	CameraRootElement->FirstChildElement( "Position" )->FirstChildElement( "z" )->QueryFloatText( &( Eye.z ) );

	CameraRootElement->FirstChildElement( "Target" )->FirstChildElement( "x" )->QueryFloatText( &( Target.x ) );
	CameraRootElement->FirstChildElement( "Target" )->FirstChildElement( "y" )->QueryFloatText( &( Target.y ) );
	CameraRootElement->FirstChildElement( "Target" )->FirstChildElement( "z" )->QueryFloatText( &( Target.z ) );

	CameraRootElement->FirstChildElement( "ApertureResolution" )->FirstChildElement( "width" )->QueryFloatText( &( ApertureResolution.x ) );
	CameraRootElement->FirstChildElement( "ApertureResolution" )->FirstChildElement( "height" )->QueryFloatText( &( ApertureResolution.y ) );

	CameraRootElement->FirstChildElement( "ApertureDistance" )->QueryFloatText( &ApertureDistance );

	CameraRootElement->FirstChildElement( "ExposureTime" )->QueryFloatText( &ExposureTime );

	CameraRootElement->FirstChildElement( "ViewDistance" )->QueryFloatText( &ViewDistance );

	UpdateProperty();
}