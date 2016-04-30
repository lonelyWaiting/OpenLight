#pragma once

#include "SurfaceIntegrator.h"

class SimpleIntegrator : public SurfaceIntegrator
{
	DECLARE_DYNAMIC_CREATE_DERIVED( SimpleIntegrator , SurfaceIntegrator )

public:
	Spectrum Li( const Scene* scene , const Renderer* renderer , IntersectRecord* record , Ray* ray , Accelerator* pAccelerator ) const;

	void SetMaxRecusiveDepth( int maxdepth );

	void Deserialization(XMLElement* IntegratorRootElement);

private:
	int MaxDepth;
	int SamplePathNumber;
};