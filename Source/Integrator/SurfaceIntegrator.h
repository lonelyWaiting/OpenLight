#pragma once

#include "Spectrum/Spectrum.h"
#include "Core/VSerializableObject.h"
#include "Core/VCustomRTTI.h"

class Scene;
class Renderer;
class IntersectRecord;
class Ray;
class Accelerator;

class SurfaceIntegrator : public VSerializableObject
{
	DECLARE_DYNAMIC_CREATE_BASE( SurfaceIntegrator )

public:
	virtual Spectrum Li( const Scene* scene , const Renderer* renderer , IntersectRecord* record , Ray* ray , Accelerator* pAccelerator ) const = 0;

	virtual void Deserialization(XMLElement* IntegratorRootElement)= 0;
};