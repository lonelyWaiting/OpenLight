#pragma once

#include "Math/Ray.h"
#include "Math/ONB.h"
#include "Film/Film.h"
#include "Math/Vector3.h"
#include "Sampler/CameraSample.h"
#include "Core/VCustomRTTI.h"
#include "Core/VSerializableObject.h"

namespace tinyxml2
{
	class XMLElement;
}

class Scene;

class Camera : public VSerializableObject
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

	void SetFilm( Film* _film );
	
public:
	Point3f GetCameraPosition() const;

	double GetViewDistance() const;

	double GetExposureTime() const;

	Film* GetFilm() const;

	void SetFovy( double fov );

public:
	virtual Ray GenerateRay( double RasterX, double RasterY , const CameraSample& SamplePoint ) = 0;

	virtual void UpdateProperty();

protected:
	Point3f		Eye;						// 相机中心
	Point3f		Target;						// 视线方向
	double		ViewDistance;				// 到视平面的距离
	double		ExposureTime;				// 曝光时间
	double		fovy;						// y方向的fov

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