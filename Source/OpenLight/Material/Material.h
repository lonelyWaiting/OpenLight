#pragma once

#include "Math/Point3.h"
#include "BRDF/BxDF.h"
#include "BRDF/BSDF.h"
#include "Core/VSerializableObject.h"
#include "Core/VCustomRTTI.h"

class Material : public VSerializableObject
{
	DECLARE_DYNAMIC_CREATE_BASE( Material )

public:
	Material();

	virtual BSDF* GetBSDF( const Vector2f& uv , const Point3f& point , const Vector3f& normal ) const = 0;

	virtual ~Material();
};