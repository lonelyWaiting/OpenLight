#pragma once

#include "Math/Ray.h"
#include "Math/ONB.h"
#include "Film/Film.h"
#include "Math/Vector3.h"
#include "Sampler/CameraSample.h"

class Scene;

class Camera
{
public:
	enum FitMode
	{
		FILL ,
		OverScan
	};

public:
	Camera();

	Camera( const Camera& orig );

	void SetCameraPositionAndLookAt( const Point3f& eye , const Point3f& TargetPoint );

	void SetApertureDistance( float d );

	void SetViewDistance( float d );

	void SetApertureResolution( const Vector2f& resolution );
	
	void SetExposureTime( float ExposureTime );

	void SetFitMode( FitMode _Mode );

	void SetFilm( Film* _film );
	
public:
	Point3f GetCameraPosition() const;

	Vector2f GetApertureResolution() const;

	float GetViewDistance() const;

	float GetApertureDistance() const;

	float GetExposureTime() const;

	Film* GetFilm() const;

public:
	virtual Ray GenerateRay( float RasterX, float RasterY , const CameraSample& SamplePoint ) = 0;

	virtual void UpdateProperty();

protected:
	Point3f		Eye;						// 相机中心
	Point3f		Target;						// 视线方向
	float		ViewDistance;				// 到视平面的距离
	float		ApertureDistance;			// 到光圈的距离
	Vector2f	ApertureResolution;			// 光圈大小
	float		ExposureTime;				// 曝光时间
	FitMode		Mode;

protected:
	bool		NeedUpdateParams;
	float		Top;
	float		Bottom;
	float		Left;
	float		Right;
	float		FilmAspectRatio;
	
	ONB uvw;							// 坐标系

protected:
	Camera& operator = ( const Camera& camera );

private:
	Film * film;
};

typedef std::shared_ptr<Camera> CameraPtr;