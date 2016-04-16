#pragma once

#include "Sampler.h"

class NRooksSampler : public Sampler
{
public:
	NRooksSampler();

	NRooksSampler( const NRooksSampler& rhs );

	NRooksSampler& operator = ( const NRooksSampler& rhs );

	virtual ~NRooksSampler();

public:
	virtual void ParseSampler( XMLElement* SamplerRootElement );

private:
	virtual void GenerateUnitSquareSamples();
};