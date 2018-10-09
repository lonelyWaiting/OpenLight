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
	Target             = orig.Target;
	uvw                = orig.uvw;
	ExposureTime       = orig.ExposureTime;
	NearPlane          = orig.NearPlane;
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

Vector3f Camera::GetViewDir() const
{
	return Target - Eye;
}

float Camera::GetExposureTime() const
{
	return ExposureTime;
}

Film* Camera::GetFilm() const
{
	return film;
}

void Camera::SetFovy( float fov )
{
	fovy = fov;
	UpdateProperty();
}

float Camera::GetFovy() const
{
	return fovy;
}

void Camera::UpdateProperty()
{
	Vector3f look = Target - Eye;

	uvw.InitFromW(look);
	
	float tanfovYinv2 = tan( ToRadians( fovy ) );

	Top = tanfovYinv2;
	Bottom = -Top;

	Right = Top * FilmAspectRatio;
	Left = -Right;
}