#include "Utilities/PCH.h"
#include "Math/Point2.h"
#include "Math/Point3.h"
#include "Math/Vector2.h"
#include "Math/Ray.h"
#include "Camera.h"
#include "tinyxml2.h"

Camera::Camera()
{

}

Camera::Camera( const Camera& orig )
{
	Eye                = orig.Eye;
	ViewDistance       = orig.ViewDistance;
	Target             = orig.Target;
	uvw                = orig.uvw;
	ExposureTime       = orig.ExposureTime;
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

double Camera::GetViewDistance() const
{
	return ViewDistance;
}

double Camera::GetExposureTime() const
{
	return ExposureTime;
}

Film* Camera::GetFilm() const
{
	return film;
}

void Camera::SetFovy( double fov )
{
	fovy = fov;
	UpdateProperty();
}

void Camera::UpdateProperty()
{
	Vector3f look = Target - Eye;

	uvw.InitFromW(look);
	
	double tanfovYinv2 = tan( ToRadians( fovy / 2.0 ) );

	Top = ViewDistance * tanfovYinv2;
	Bottom = -Top;

	Right = Top * FilmAspectRatio;
	Left = -Right;
}