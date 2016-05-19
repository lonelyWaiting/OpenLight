#pragma once

#include "SurfaceIntegrator.h"

namespace tinyxml2
{
	class XMLElement;
}

class PathIntegrator : public SurfaceIntegrator
{
	DECLARE_DYNAMIC_CREATE_DERIVED( PathIntegrator , SurfaceIntegrator )

public:
	Spectrum Li( const Scene* scene , const Renderer* renderer , IntersectRecord* record , Ray* ray , Accelerator* pAccelerator ) const;

public:
	void Deserialization( tinyxml2::XMLElement* IntegratorRootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );
};