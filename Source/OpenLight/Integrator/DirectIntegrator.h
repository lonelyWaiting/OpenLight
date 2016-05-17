#pragma once

#include "SurfaceIntegrator.h"
#include "Utilities/PCH.h"
#include "Spectrum/Spectrum.h"
#include "BRDF/BxDFType.h"

namespace tinyxml2
{
	class XMLElement;
}

class Light;
class BSDF;
class Point3f;
class Vector3f;
class Normal;

class DirectIntegrator : public SurfaceIntegrator
{
	DECLARE_DYNAMIC_CREATE_DERIVED( DirectIntegrator , SurfaceIntegrator )

public:
	Spectrum Li( const Scene* scene , const Renderer* renderer , IntersectRecord* record , Ray* ray , Accelerator* pAccelerator ) const;

public:
	void Deserialization( tinyxml2::XMLElement* IntegratorRootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );
};

Spectrum UniformSampleOneLight( const Scene* scene , const Renderer* renderer , Accelerator* pAccelerator , BSDF* bsdf , const Point3f& point , const Vector3f& wo , const Normal& n );

Spectrum EstimateDirect( const Scene* scene , const Renderer* renderer , Accelerator* pAccelerator , Light* light , BSDF* bsdf , const Point3f& point , const Vector3f& wo , const Normal& n , BxDFType flags );