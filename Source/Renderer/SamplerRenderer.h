#pragma once

#include "Renderer.h"

class Camera;

class SamplerRenderer : public Renderer
{
public:
	SamplerRenderer( Sampler* sampler , Camera* camera );

	void Render( const Scene* scene );

	virtual Spectrum Li( const Scene* scene ,
						 const Sampler* sample ,
						 RandomNumberGenerator& rng ,
						 IntersectRecord* record = nullptr ,
						 Spectrum* T = nullptr ) const;

	virtual Spectrum Transmittance( const Scene* scene ,
									const Sampler* sample ,
									RandomNumberGenerator& rng ) const;
private:
	Sampler* sampler;
	Camera* camera;
};