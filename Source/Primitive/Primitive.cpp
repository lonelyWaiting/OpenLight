#include "PCH.h"
#include "Primitive.h"

Primitive::Primitive()
{

}

Primitive::~Primitive()
{

}

void Primitive::SetShape( Shape* _shape )
{
	shape = _shape;
}

bool Primitive::Intersect( const Ray& r , IntersectRecord* record ) const
{
	record->primitivePtr = this;

	if( shape->Intersect( r , record ) )
	{
		record->HitPointColor = color;

		return true;
	}

	return false;
}

bool Primitive::IntersectP( const Ray& r ) const
{
	return shape->IntersectP( r );
}

void Primitive::SetDiffuseColor( const Spectrum& _color )
{
	color = _color;
}