#pragma once

#include "Renderer.h"

namespace tinyxml2
{
	class XMLElement;
}

class SamplerRenderer : public Renderer
{
	DECLARE_DYNAMIC_CREATE_DERIVED( SamplerRenderer , Renderer )

public:
	SamplerRenderer();

	SamplerRenderer(Sampler* sampler, Camera* camera, SurfaceIntegrator* _surfaceIntegrator, Accelerator* _pAccelerator , int _spp = 8);

	void SetProperty( Sampler* sampler , Camera* camera , SurfaceIntegrator* _surfaceIntegrator , Accelerator* _pAccelerator );

	virtual ~SamplerRenderer();

	void Render( const Scene* scene );

	virtual Spectrum Li(const Scene* scene,
						Ray* ray ,
						IntersectRecord* record = nullptr ) const;

public:
	void Deserialization( tinyxml2::XMLElement* RendererRootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );

private:
	Sampler* sampler;
	Camera* camera;
	SurfaceIntegrator* surfaceIntegrator;
	Accelerator* pAccelerator;
};