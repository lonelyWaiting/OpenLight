#include "Utilities/PCH.h"
#include "Primitive/Primitive.h"
#include "Light/Light.h"
#include "IntersectRecord.h"
#include "Math/Vector2.h"
#include "Math/Transform.h"

BSDF* IntersectRecord::GetBSDF()
{
	return primitivePtr->GetBSDF( uv , HitPoint , normal );
}

Spectrum IntersectRecord::Le( const Vector3f& wo )
{
	if( primitivePtr->GetAreaLight() )
	{
		return primitivePtr->GetAreaLight()->Le( HitPoint , normal , wo );
	}

	return Spectrum( 0 );
}