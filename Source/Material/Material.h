#pragma once

#include "Math/Point3.h"
#include "Math/Normal.h"
#include "BRDF/BxDF.h"

class Material
{
public:
	virtual BxDF* GetBxDF( const Point3f& point , const Normal& normal ) const = 0;

	virtual ~Material();
};