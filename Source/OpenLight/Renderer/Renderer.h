#pragma once

#include "Spectrum/Spectrum.h"
#include "Core/VCustomRTTI.h"
#include "Core/VSerializableObject.h"

class Scene;
class Sampler;
class RNG;
struct IntersectRecord;
class Camera;
class SurfaceIntegrator;
class Accelerator;
namespace tinyxml2
{
	class XMLElement;
}

class Renderer : public VSerializableObject
{
	DECLARE_DYNAMIC_CREATE_BASE( Renderer )

public:
	virtual ~Renderer();

	Renderer();

	Renderer( int spp );

	virtual bool Render( const Scene* scene ) = 0;

	virtual void SetProperty( Sampler* _sampler , Camera* _camera , SurfaceIntegrator* _surfaceIntegrator , Accelerator* _pAccelerator ) = 0;

	virtual Spectrum Li( const Scene* scene ,
						 Rayf* ray ,
						 IntersectRecord* record = nullptr) const = 0;

	void SetSpp( int spp );

	int GetSpp() const;

	virtual void ResetRender() = 0;

	virtual Accelerator* GetAccelerator() = 0;

protected:
	int spp;		// sample per pixel
};