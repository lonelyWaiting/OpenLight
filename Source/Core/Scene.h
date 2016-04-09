#pragma once

#include "PCH.h"
#include "Primitive/Primitive.h"
#include "Math/Ray.h"
#include "Spectrum/Spectrum.h"

class Scene
{
public:
	Scene();

	void AddObject( Primitive& prim );

	Spectrum Trace( const Ray& ray ) const;

protected:
	std::vector<Primitive> Objects;
};