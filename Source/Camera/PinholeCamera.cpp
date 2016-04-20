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

Ray PinholeCamera::GenerateRay( double RasterX , double RasterY , const CameraSample& SamplePoint )
{
	Vector2f RasterResolution = GetFilm()->GetResolution();

	// 视平面上的采样点
	double x = ( RasterX + SamplePoint.ImageSamples.x ) / RasterResolution.x  * ( Right - Left ) + Left;
	double y = ( RasterY + SamplePoint.ImageSamples.y ) / RasterResolution.y * ( Top - Bottom ) + Bottom;

	Vector3f dir = x * uvw.U + y * uvw.V + ViewDistance * uvw.W;

	return Ray( Eye , Normalize( dir ) , 1e-3f );
}

void PinholeCamera::Deserialization( XMLElement* CameraRootElement )
{
	CameraRootElement->FirstChildElement( "Position" )->FirstChildElement( "x" )->QueryDoubleText( &( Eye.x ) );
	CameraRootElement->FirstChildElement( "Position" )->FirstChildElement( "y" )->QueryDoubleText( &( Eye.y ) );
	CameraRootElement->FirstChildElement( "Position" )->FirstChildElement( "z" )->QueryDoubleText( &( Eye.z ) );

	CameraRootElement->FirstChildElement( "Target" )->FirstChildElement( "x" )->QueryDoubleText( &( Target.x ) );
	CameraRootElement->FirstChildElement( "Target" )->FirstChildElement( "y" )->QueryDoubleText( &( Target.y ) );
	CameraRootElement->FirstChildElement( "Target" )->FirstChildElement( "z" )->QueryDoubleText( &( Target.z ) );

	CameraRootElement->FirstChildElement( "ApertureResolution" )->FirstChildElement( "width" )->QueryDoubleText( &( ApertureResolution.x ) );
	CameraRootElement->FirstChildElement( "ApertureResolution" )->FirstChildElement( "height" )->QueryDoubleText( &( ApertureResolution.y ) );

	CameraRootElement->FirstChildElement( "ApertureDistance" )->QueryDoubleText( &ApertureDistance );

	CameraRootElement->FirstChildElement( "ExposureTime" )->QueryDoubleText( &ExposureTime );

	CameraRootElement->FirstChildElement( "ViewDistance" )->QueryDoubleText( &ViewDistance );

	UpdateProperty();
}