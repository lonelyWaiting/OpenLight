#include "PCH.h"
#include "Sampler/Sampling.h"
#include "ThinLensCamera.h"

ThinLensCamera::ThinLensCamera()
{

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

Ray ThinLensCamera::GenerateRay( float RasterX , float RasterY , const CameraSample& SamplePoint )
{
	Vector2f RasterResolution = GetFilm()->GetResolution();

	// x , y为视平面上的采样点
	Point2f ImageSamples;
	ImageSamples.x = ( RasterX + SamplePoint.ImageSamples.x ) / RasterResolution.x * ( Right - Left ) + Left;
	ImageSamples.y = ( RasterY + SamplePoint.ImageSamples.y ) / RasterResolution.y  * ( Top - Bottom ) + Bottom;

	float x = ImageSamples.x / ViewDistance * LensFocus;
	float y = ImageSamples.y / ViewDistance * LensFocus;

	Point2f LensSamples = MapUnitSquareToUnitDisk( SamplePoint.LensSamples );

	Point3f Orig = Eye + LensRadius * LensSamples.x * uvw.U + LensRadius * LensSamples.y * uvw.V;

	Vector3f dir = ( x - LensRadius * LensSamples.x ) * uvw.U + ( y - LensRadius * LensSamples.y ) * uvw.V + LensFocus * uvw.W;

	return Ray( Orig , Normalize( dir ) , 1e-3f );
}

void ThinLensCamera::ParseCamera( XMLElement* CameraRootElement )
{
	CameraRootElement->FirstChildElement( "Position" )->FirstChildElement( "x" )->QueryFloatText( &( Eye.x ) );
	CameraRootElement->FirstChildElement( "Position" )->FirstChildElement( "y" )->QueryFloatText( &( Eye.y ) );
	CameraRootElement->FirstChildElement( "Position" )->FirstChildElement( "z" )->QueryFloatText( &( Eye.z ) );

	CameraRootElement->FirstChildElement( "Target" )->FirstChildElement( "x" )->QueryFloatText( &( Target.x ) );
	CameraRootElement->FirstChildElement( "Target" )->FirstChildElement( "y" )->QueryFloatText( &( Target.y ) );
	CameraRootElement->FirstChildElement( "Target" )->FirstChildElement( "z" )->QueryFloatText( &( Target.z ) );

	CameraRootElement->FirstChildElement( "LensFocus" )->QueryFloatText( &LensFocus );

	CameraRootElement->FirstChildElement( "LensRadius" )->QueryFloatText( &LensRadius );

	CameraRootElement->FirstChildElement( "ApertureResolution" )->FirstChildElement( "width" )->QueryFloatText( &( ApertureResolution.x) );
	CameraRootElement->FirstChildElement( "ApertureResolution" )->FirstChildElement( "height" )->QueryFloatText( &( ApertureResolution.y ) );

	CameraRootElement->FirstChildElement( "ApertureDistance" )->QueryFloatText( &ApertureDistance );

	CameraRootElement->FirstChildElement( "ExposureTime" )->QueryFloatText( &ExposureTime );

	CameraRootElement->FirstChildElement( "ViewDistance" )->QueryFloatText( &ViewDistance );

	UpdateProperty();
}