#pragma once

class PinholeCamera : public Camera
{
	DECLARE_DYNAMIC_CREATE_DERIVED( PinholeCamera , Camera )

public:
	PinholeCamera();

	PinholeCamera( const PinholeCamera& rhs );

public:
	Ray GenerateRay( double RasterX , double RasterY , const CameraSample& SamplePoint );

	void Deserialization( XMLElement* CameraRootElement );
};