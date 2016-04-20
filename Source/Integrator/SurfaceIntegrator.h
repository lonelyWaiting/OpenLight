#pragma once

#include "Spectrum/Spectrum.h"
#include "Core/VObject.h"

class Scene;
class Renderer;
class IntersectRecord;
class Ray;

class SurfaceIntegrator : public VObject
{
public:
	virtual Spectrum Li( const Scene* scene , const Renderer* renderer , IntersectRecord* record , Ray* ray ) const = 0;

	virtual void Deserialization(XMLElement* IntegratorRootElement)= 0;
};

void SpecularReflect( const Ray& ray , const Scene* scene , const Renderer* renderer , IntersectRecord* record );