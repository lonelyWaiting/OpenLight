#pragma once
#include "Sampler.h"

namespace tinyxml2
{
	class XMLElement;
}

class PureRandomSampler : public Sampler
{
	DECLARE_DYNAMIC_CREATE_DERIVED( PureRandomSampler , Sampler )

public:
	PureRandomSampler();

	PureRandomSampler( const PureRandomSampler& r );

	PureRandomSampler& operator = ( const PureRandomSampler& rhs );

public:
	void Deserialization( tinyxml2::XMLElement* SamplerRootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );

private:
	void GenerateUnitSquareSamples();
};