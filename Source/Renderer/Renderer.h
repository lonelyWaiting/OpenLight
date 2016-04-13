#pragma once

#include "Spectrum/Spectrum.h"

class Scene;
class Sampler;
class RandomNumberGenerator;
class IntersectRecord;
class Ray;

class Renderer
{
public:
	virtual ~Renderer();

	virtual void Render( const Scene* scene ) = 0;

	virtual Spectrum Li( const Scene* scene ,
						 Ray* ray ,
						 IntersectRecord* record = nullptr) const = 0;
};