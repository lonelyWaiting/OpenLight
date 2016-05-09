#include "Utilities/PCH.h"
#include "VIntersectable.h"

Bound3f VIntersectable::GetObjectBoundingBox() const
{
	return BBoxLocal;
}

Bound3f VIntersectable::GetWorldBoundingBox() const
{
	return BBoxWorld;
}