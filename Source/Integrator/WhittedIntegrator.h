#pragma once

#include "SurfaceIntegrator.h"

class WhittedIntegrator : public SurfaceIntegrator
{
	DECLARE_DYNAMIC_CREATE_DERIVED( WhittedIntegrator , SurfaceIntegrator )

public:
	Spectrum Li( const Scene* scene , const Renderer* renderer , IntersectRecord* record , Ray* ray , Accelerator* pAccelerator ) const;

	void SetMaxRecusiveDepth( int maxdepth );

	void Deserialization( XMLElement* IntegratorRootElement );

private:
	int mMaxDepth;
};