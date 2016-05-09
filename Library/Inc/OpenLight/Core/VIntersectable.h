#pragma once

#include "Math/Bound3.h"

class VIntersectable
{
public:
	virtual Bound3f GetWorldBoundingBox() const;

	virtual Bound3f GetObjectBoundingBox() const;

protected:
	Bound3f BBoxLocal;
	Bound3f BBoxWorld;
};