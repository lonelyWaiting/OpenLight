#pragma once

#include "Spectrum/Spectrum.h"

class Scene;
class Sampler;
class RandomNumberGenerator;
class IntersectRecord;

class Renderer
{
public:
	virtual ~Renderer();

	virtual void Render( const Scene* scene ) = 0;

	virtual Spectrum Li( const Scene* scene ,
						 const Sampler* sampler ,
						 RandomNumberGenerator& rng ,
						 IntersectRecord* record = nullptr ,
						 Spectrum* T = nullptr ) const = 0;

	virtual Spectrum Transmittance( const Scene* scene ,
									const Sampler* sampler ,
									RandomNumberGenerator& rng ) const = 0;
};