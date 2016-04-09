#pragma once
#include "Sampler.h"

class PureRandomSampler : public Sampler
{
public:
	PureRandomSampler();

	PureRandomSampler( const int num );

	PureRandomSampler( const PureRandomSampler& r );

	PureRandomSampler& operator = ( const PureRandomSampler& rhs );

	virtual ~PureRandomSampler();

	virtual PureRandomSampler* clone() const;

private:
	virtual void GenerateUnitSquareSamples();
};