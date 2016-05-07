#pragma once

#include "Spectrum/Spectrum.h"
#include "Core/VSerializableObject.h"
#include "Core/VCustomRTTI.h"

class Scene;
class Renderer;
struct IntersectRecord;
class Ray;
class Accelerator;
namespace tinyxml2
{
	class XMLElement;
}

class SurfaceIntegrator : public VSerializableObject
{
	DECLARE_DYNAMIC_CREATE_BASE( SurfaceIntegrator )

public:
	virtual Spectrum Li( const Scene* scene , const Renderer* renderer , IntersectRecord* record , Ray* ray , Accelerator* pAccelerator ) const = 0;

	virtual void Deserialization( tinyxml2::XMLElement* IntegratorRootElement ) = 0;

	void SetMaxRecusiveDepth( int MaxDepth );

protected:
	int mMaxDepth;
};