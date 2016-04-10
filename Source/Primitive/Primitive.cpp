#include "PCH.h"
#include "Shapes/Sphere.h"
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

void Primitive::ParsePrimitive( XMLElement* PrimitiveRootElment )
{
	XMLElement* PrimitiveShapeElement = PrimitiveRootElment->FirstChildElement( "shape" );

	const char* ShapeType = PrimitiveShapeElement->FirstChildElement( "type" )->GetText();

	if( !std::strcmp( "sphere" , ShapeType ) )
	{
		shape = new Sphere;
		shape->ParseShape( PrimitiveShapeElement );
	}
	else
	{
		Assert( "don't support \'%s\' shape object" , ShapeType );
	}

	float r , g , b;
	XMLElement* MaterialElement = PrimitiveRootElment->FirstChildElement( "material" );
	XMLElement* MaterialSurfaceColorElement = MaterialElement->FirstChildElement( "color" );
	MaterialSurfaceColorElement->FirstChildElement( "r" )->QueryFloatText( &r );
	MaterialSurfaceColorElement->FirstChildElement( "g" )->QueryFloatText( &g );
	MaterialSurfaceColorElement->FirstChildElement( "b" )->QueryFloatText( &b );

	color = Spectrum::FromRGB( r , g , b );
}