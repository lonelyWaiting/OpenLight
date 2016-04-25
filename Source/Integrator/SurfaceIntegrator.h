#pragma once

#include "Spectrum/Spectrum.h"
#include "Core/VSerializableObject.h"

class Scene;
class Renderer;
class IntersectRecord;
class Ray;
class Accelerator;

class SurfaceIntegrator : public VSerializableObject
{
public:
	virtual Spectrum Li( const Scene* scene , const Renderer* renderer , IntersectRecord* record , Ray* ray , Accelerator* pAccelerator ) const = 0;

	virtual void Deserialization(XMLElement* IntegratorRootElement)= 0;
};