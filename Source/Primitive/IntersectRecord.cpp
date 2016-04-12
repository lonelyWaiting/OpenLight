#include "PCH.h"
#include "Primitive/Primitive.h"
#include "IntersectRecord.h"

BxDF* IntersectRecord::GetBxDF()
{
	return primitivePtr->GetBxDF( HitPoint , normal );
}