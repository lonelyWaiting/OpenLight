#pragma once

#include "Spectrum/Spectrum.h"
#include "Math/Point3.h"
#include "BRDF/BSDF.h"
#include "Math/MathHelper.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Transform.h"

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
	float HitT;
	Point3f HitPoint;
	Vector3f normal;
	Vector2f uv;
};