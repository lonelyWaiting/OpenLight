#pragma once

#include "Spectrum/Spectrum.h"
#include "Core/VCustomRTTI.h"

class Scene;
class Sampler;
class RandomNumberGenerator;
class IntersectRecord;
class Ray;
class Camera;
class SurfaceIntegrator;
class Accelerator;

class Renderer
{
	DECLARE_DYNAMIC_CREATE_BASE( Renderer )

public:
	virtual ~Renderer();

	virtual void Render( const Scene* scene ) = 0;

	virtual void SetProperty( Sampler* _sampler , Camera* _camera , SurfaceIntegrator* _surfaceIntegrator , Accelerator* _pAccelerator ) = 0;

	virtual Spectrum Li( const Scene* scene ,
						 Ray* ray ,
						 IntersectRecord* record = nullptr) const = 0;

public:
	virtual void Deserialization( XMLElement* RendererRootElement ) = 0;
};