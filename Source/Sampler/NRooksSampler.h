#pragma once

#include "Sampler.h"

class NRooksSampler : public Sampler
{
	DECLARE_DYNAMIC_CREATE_DERIVED( NRooksSampler , Sampler )

public:
	NRooksSampler();

	NRooksSampler( const NRooksSampler& rhs );

	NRooksSampler& operator = ( const NRooksSampler& rhs );

	virtual ~NRooksSampler();

public:
	virtual void Deserialization( XMLElement* SamplerRootElement );

private:
	virtual void GenerateUnitSquareSamples();
};