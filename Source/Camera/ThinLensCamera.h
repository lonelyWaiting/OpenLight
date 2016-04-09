#pragma once

#include "Camera.h"
#include "Math/Point2.h"
#include "Math/Point3.h"
#include "Math/Vector2.h"
#include "Film/Film.h"
#include "Sampler/Sampler.h"
#include "Core/Scene.h"

class ThinLensCamera : public Camera
{
public:
	ThinLensCamera();

	ThinLensCamera( const ThinLensCamera& rhs );

	void SetLensRadius( float _LensRadius );

	void SetLensFocus( float _LensFocus );

public:
	Ray GenerateRay( float RasterX , float RasterY , const CameraSample& SamplePoint );

private:
	float LensRadius;			// Í¸¾µ°ë¾¶
	float LensFocus;			// ½¹¾à
};

typedef std::shared_ptr<ThinLensCamera> ThinLensPtr;