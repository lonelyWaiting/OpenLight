#pragma once

namespace tinyxml2
{
	class XMLDocument;
	class XMLElement;
}
class PinholeCamera : public Camera
{
	DECLARE_DYNAMIC_CREATE_DERIVED( PinholeCamera , Camera )

public:
	PinholeCamera();

	PinholeCamera( const PinholeCamera& rhs );

public:
	Ray GenerateRay( double RasterX , double RasterY , const CameraSample& SamplePoint );

	void Deserialization( tinyxml2::XMLElement* CameraRootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );
};