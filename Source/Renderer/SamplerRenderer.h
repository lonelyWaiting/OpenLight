#pragma once

#include "Renderer.h"

class Camera;
class SurfaceIntegrator;

class SamplerRenderer : public Renderer
{
public:
	SamplerRenderer(Sampler* sampler, Camera* camera, SurfaceIntegrator* _surfaceIntegrator, int _spp = 8);

	virtual ~SamplerRenderer();

	void Render( const Scene* scene );

	virtual Spectrum Li(const Scene* scene,
						Ray* ray ,
						IntersectRecord* record = nullptr ) const;

public:
	void Deserialization( XMLElement* RendererRootElement );

private:
	Sampler* sampler;
	Camera* camera;
	SurfaceIntegrator* surfaceIntegrator;
	int spp;		// sample per pixel
};