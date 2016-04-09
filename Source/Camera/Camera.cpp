#include "PCH.h"
#include "Math/Point2.h"
#include "Math/Point3.h"
#include "Math/Vector2.h"
#include "Math/Ray.h"
#include "Camera.h"

Camera::Camera()
{
	NeedUpdateParams = false;

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
	NeedUpdateParams   = orig.NeedUpdateParams;
	Mode			   = orig.Mode;
}

void Camera::SetCameraPositionAndLookAt( const Point3f& eye , const Point3f& TargetPoint )
{
	Eye = eye;

	Target = TargetPoint;

	uvw.InitFromW( Vector3f( Target - Eye ) );

	NeedUpdateParams = true;
}

void Camera::SetExposureTime( float _ExposureTime )
{
	if( ExposureTime != _ExposureTime )
	{
		ExposureTime = _ExposureTime;

		NeedUpdateParams = true;
	}
}

void Camera::SetApertureResolution( const Vector2f& resolution )
{
	if( ApertureResolution != resolution )
	{
		ApertureResolution = resolution;

		NeedUpdateParams = true;
	}	
}

void Camera::SetApertureDistance(float d)
{ 
	if( ApertureDistance != d )
	{
		ApertureDistance = d;

		NeedUpdateParams = true;
	}
}

void Camera::SetViewDistance( float d )
{
	if( ViewDistance != d )
	{
		ViewDistance = d;

		NeedUpdateParams = true;
	}
}

void Camera::SetFitMode( FitMode _Mode )
{
	if( Mode != _Mode )
	{
		Mode = _Mode;

		NeedUpdateParams = true;
	}
}

void Camera::SetFilm( Film* _film )
{
	if( film != _film )
	{
		film = _film;

		FilmAspectRatio = film->GetAspectio();

		NeedUpdateParams = true;
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

float Camera::GetViewDistance() const
{
	return ViewDistance;
}

float Camera::GetExposureTime() const
{
	return ExposureTime;
}

float Camera::GetApertureDistance() const
{
	return ApertureDistance;
}

Film* Camera::GetFilm() const
{
	return film;
}

void Camera::UpdateProperty()
{
	if( NeedUpdateParams )
	{
		float ApertureAspectRatio = ApertureResolution.x / ApertureResolution.y;

		Top = ( ApertureResolution.y * inchToMm * 0.5f ) * ViewDistance / ApertureDistance;

		Right = ( ApertureResolution.x * inchToMm * 0.5f ) * ViewDistance / ApertureDistance;

		float xScale = 1.0f;
		float yScale = 1.0f;

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

		NeedUpdateParams = false;
	}
}