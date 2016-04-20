#include "PCH.h"
#include "IO/Log.h"
#include "Math/Normal.h"
#include "Math/Point3.h"
#include "Math/Ray.h"
#include "Math/Transform.h"
#include "Math/Vector3.h"
#include "Shape.h"

Shape::Shape(Spectrum _emmisive /*= Spectrum(0)*/)
{
	ObjectToWorld = new Transform;

	WorldToObject = new Transform;

	emmisive = _emmisive;

	bCombination = false;
}

Shape::Shape( const Transform* _ObjectToWorld )
{
	// Éî¿½±´
	memcpy( ObjectToWorld , _ObjectToWorld , sizeof( Transform ) );

	*WorldToObject = Inverse( *ObjectToWorld );
}

Shape::~Shape()
{
	SAFE_DELETE( ObjectToWorld );
	SAFE_DELETE( WorldToObject );
}

bool Shape::Intersect( Ray& ray , IntersectRecord* record ) const
{
	Assert( "Called Unimplemented Shape::Intersect()" );

	return false;
}

bool Shape::IsCombinationShape()
{
	return bCombination;
}

int Shape::GetSubShapeCount() const
{
	return 0;
}

Shape* Shape::GetSubShape( int index ) const
{
	return nullptr;
}