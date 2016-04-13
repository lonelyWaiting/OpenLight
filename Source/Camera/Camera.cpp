#include "PCH.h"
#include "Math/Point2.h"
#include "Math/Point3.h"
#include "Math/Vector2.h"
#include "Math/Ray.h"
#include "Camera.h"

Camera::Camera()
{
	Mode = OverScan;
}

Camera::Camera( const Camera& orig )
{
	Eye                = orig.Eye;
	ViewDistance       = orig.ViewDistance;
	ApertureDistance   = orig.ApertureDistance;
	Target             = orig.Target;
	ApertureResolution = orig.ApertureResolution;
	uvw                = orig.uvw;
	ExposureTime       = orig.ExposureTime;
	Mode			   = orig.Mode;
}

void Camera::SetFitMode( FitMode _Mode )
{
	if( Mode != _Mode )
	{
		Mode = _Mode;
	}
}

void Camera::SetFilm( Film* _film )
{
	if( film != _film )
	{
		film = _film;

		FilmAspectRatio = film->GetAspectio();
	}
}

Point3f Camera::GetCameraPosition() const
{
	return Eye;
}

Vector2f Camera::GetApertureResolution() const
{
	return ApertureResolution;
}

double Camera::GetViewDistance() const
{
	return ViewDistance;
}

double Camera::GetExposureTime() const
{
	return ExposureTime;
}

double Camera::GetApertureDistance() const
{
	return ApertureDistance;
}

Film* Camera::GetFilm() const
{
	return film;
}

void Camera::UpdateProperty()
{
	Vector3f look = Target - Eye;

	uvw.InitFromW(look);
	
	double ApertureAspectRatio = ApertureResolution.x / ApertureResolution.y;

	Top = ( ApertureResolution.y * inchToMm * 0.5f ) * ViewDistance / ApertureDistance;

	Right = ( ApertureResolution.x * inchToMm * 0.5f ) * ViewDistance / ApertureDistance;

	double xScale = 1.0f;
	double yScale = 1.0f;

	switch( Mode )
	{
		case OverScan:			// 扩大Film
		{
			if( ApertureAspectRatio > FilmAspectRatio )
			{
				yScale = ApertureAspectRatio / FilmAspectRatio;
			}
			else
			{
				xScale = FilmAspectRatio / ApertureAspectRatio;
			}
			break;
		}

		default:		// 将Film中的内容全部呈现在Image中
		{
			if( ApertureAspectRatio > FilmAspectRatio )
			{
				xScale = FilmAspectRatio / ApertureAspectRatio;
			}
			else
			{
				yScale = ApertureAspectRatio / FilmAspectRatio;
			}
			break;
		}
	}

	Right *= xScale;
	Top *= yScale;

	Bottom = -Top;
	Left = -Right;
}