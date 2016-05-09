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
	void Deserialization( tinyxml2::XMLElement* SamplerRootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );

private:
	virtual void GenerateUnitSquareSamples();
};