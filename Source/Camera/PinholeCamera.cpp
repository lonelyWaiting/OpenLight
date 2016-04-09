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