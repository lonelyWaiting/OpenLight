#pragma once

#include "Math/Transform.h"
#include "Spectrum/Spectrum.h"

class Primitive;

struct IntersectRecord
{
	IntersectRecord()
	{
		primitivePtr = nullptr;
	}

	const Primitive* primitivePtr;
	Transform WorldToObject;
	Transform ObjectToWorld;
	float HitT;
	Spectrum HitPointColor;
};