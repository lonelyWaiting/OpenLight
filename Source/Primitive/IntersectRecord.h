#pragma once

#include "Math/Transform.h"
#include "Spectrum/Spectrum.h"
#include "Math/Point3.h"
#include "BRDF/BxDF.h"
#include "Math/Normal.h"

class Primitive;

struct IntersectRecord
{
	IntersectRecord()
	{
		primitivePtr = nullptr;
	}

	BxDF* GetBxDF();

	const Primitive* primitivePtr;
	Transform WorldToObject;
	Transform ObjectToWorld;
	float HitT;
	Spectrum HitPointColor;
	Point3f HitPoint;
	Normal normal;
};