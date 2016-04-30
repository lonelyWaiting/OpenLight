#pragma once

#include "Math/Ray.h"
#include "Math/ONB.h"
#include "Film/Film.h"
#include "Math/Vector3.h"
#include "Sampler/CameraSample.h"
#include "Core/VCustomRTTI.h"

class Scene;

class Camera
{
	DECLARE_DYNAMIC_CREATE_BASE( Camera )

public:
	enum FitMode
	{
		FILL ,
		OverScan
	};

public:
	Camera();

	Camera( const Camera& orig );

	void SetFitMode( FitMode _Mode );

	void SetFilm( Film* _film );
	
public:
	Point3f GetCameraPosition() const;

	Vector2f GetApertureResolution() const;

	double GetViewDistance() const;

	double GetApertureDistance() const;

	double GetExposureTime() const;

	Film* GetFilm() const;

public:
	virtual Ray GenerateRay( double RasterX, double RasterY , const CameraSample& SamplePoint ) = 0;

	virtual void UpdateProperty();

public:
	virtual void Deserialization( XMLElement* CameraRootElement ) = 0;

protected:
	Point3f		Eye;						// 相机中心
	Point3f		Target;						// 视线方向
	double		ViewDistance;				// 到视平面的距离
	double		ApertureDistance;			// 到光圈的距离
	Vector2f	ApertureResolution;			// 光圈大小
	double		ExposureTime;				// 曝光时间
	FitMode		Mode;

protected:
	double		Top;
	double		Bottom;
	double		Left;
	double		Right;
	double		FilmAspectRatio;
	
	ONB uvw;							// 坐标系

protected:
	Camera& operator = ( const Camera& camera );

private:
	Film * film;
};

typedef std::shared_ptr<Camera> CameraPtr;