#include "Utilities/PCH.h"
#include "SurfaceIntegrator.h"

void SurfaceIntegrator::SetMaxRecusiveDepth( int MaxDepth )
{
	mMaxDepth = MaxDepth;
}

int SurfaceIntegrator::GetMaxRecusiveDepth() const
{
	return mMaxDepth;
}