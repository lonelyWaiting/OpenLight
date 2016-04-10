#pragma once

#include "Sampler.h"

class NRooksSampler : public Sampler
{
public:
	NRooksSampler();

	NRooksSampler( const int num_samples );

	NRooksSampler( const int num_samples , const int num_sets );

	NRooksSampler( const NRooksSampler& rhs );

	NRooksSampler& operator = ( const NRooksSampler& rhs );

	virtual ~NRooksSampler();

	virtual NRooksSampler* clone() const;

private:
	virtual void GenerateUnitSquareSamples();
};