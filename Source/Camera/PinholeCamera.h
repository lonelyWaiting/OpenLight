#pragma once

class PinholeCamera : public Camera
{
public:
	PinholeCamera();

	PinholeCamera( const PinholeCamera& rhs );

public:
	Ray GenerateRay( float RasterX , float RasterY , const CameraSample& SamplePoint );
};