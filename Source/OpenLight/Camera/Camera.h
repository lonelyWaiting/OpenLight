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

	float GetExposureTime() const;

	Film* GetFilm() const;

	void SetFovy( float fov );

	float GetFovy() const;

public:
	virtual Rayf GenerateRay( float RasterX, float RasterY , const CameraSample& SamplePoint ) = 0;

	virtual void UpdateProperty();

protected:
	Point3f		Eye;						// 相机中心
	Point3f		Target;						// 视线方向
	float		ExposureTime;				// 曝光时间
	float		fovy;						// y方向的fov
	float		NearPlane;

protected:
	float		Top;
	float		Bottom;
	float		Left;
	float		Right;
	float		FilmAspectRatio;
	
	ONBf uvw;							// 坐标系

protected:
	Camera& operator = ( const Camera& camera );

private:
	Film * film;
};

typedef std::shared_ptr<Camera> CameraPtr;