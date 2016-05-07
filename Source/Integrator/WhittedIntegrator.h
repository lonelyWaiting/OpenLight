#pragma once

#include "SurfaceIntegrator.h"

namespace tinyxml2
{
	class XMLElement;
}

class WhittedIntegrator : public SurfaceIntegrator
{
	DECLARE_DYNAMIC_CREATE_DERIVED( WhittedIntegrator , SurfaceIntegrator )

public:
	Spectrum Li( const Scene* scene , const Renderer* renderer , IntersectRecord* record , Ray* ray , Accelerator* pAccelerator ) const;

	void Deserialization( tinyxml2::XMLElement* IntegratorRootElement );
};