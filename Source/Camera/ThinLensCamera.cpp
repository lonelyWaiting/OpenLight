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

		NeedUpdateParams = true;
	}
}

void ThinLensCamera::SetLensFocus( float _LensFocus )
{
	if( LensFocus != _LensFocus )
	{
		LensFocus = _LensFocus;

		NeedUpdateParams = true;
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