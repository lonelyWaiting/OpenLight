#pragma once

#include "Camera.h"
#include "Math/Point2.h"
#include "Math/Point3.h"
#include "Math/Vector2.h"
#include "Film/Film.h"
#include "Sampler/Sampler.h"
#include "Core/Scene.h"

namespace tinyxml2
{
	class XMLElement;
}

class ThinLensCamera : public Camera
{
	DECLARE_DYNAMIC_CREATE_DERIVED( ThinLensCamera , Camera )

public:
	ThinLensCamera();

	ThinLensCamera( const ThinLensCamera& rhs );

	void SetLensRadius( double _LensRadius );

	void SetLensFocus( double _LensFocus );

public:
	Ray GenerateRay( double RasterX , double RasterY , const CameraSample& SamplePoint );

public:
	void Deserialization( tinyxml2::XMLElement* CameraRootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );

private:
	double LensRadius;			// Í¸¾µ°ë¾¶
	double LensFocus;			// ½¹¾à
};

typedef std::shared_ptr<ThinLensCamera> ThinLensPtr;