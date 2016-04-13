#pragma once

class PinholeCamera : public Camera
{
public:
	PinholeCamera();

	PinholeCamera( const PinholeCamera& rhs );

public:
	Ray GenerateRay( double RasterX , double RasterY , const CameraSample& SamplePoint );

	void ParseCamera( XMLElement* CameraRootElement );
};