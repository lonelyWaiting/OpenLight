#include "PCH.h"
#include "Primitive/Primitive.h"
#include "IntersectRecord.h"

BSDF* IntersectRecord::GetBSDF()
{
	return primitivePtr->GetBSDF( HitPoint , normal );
}