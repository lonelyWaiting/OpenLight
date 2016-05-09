#pragma once

#include "Math/Transform.h"
#include "Spectrum/Spectrum.h"
#include "Math/Point3.h"
#include "BRDF/BSDF.h"
#include "Math/Normal.h"

class Primitive;

struct IntersectRecord
{
	IntersectRecord()
	{
		primitivePtr = nullptr;
	}

	BSDF* GetBSDF();

	Spectrum Le( const Vector3f& wo );

	const Primitive* primitivePtr;
	Transform WorldToObject;
	Transform ObjectToWorld;
	double HitT;
	Point3f HitPoint;
	Normal normal;
	Spectrum SurfaceColor;
};