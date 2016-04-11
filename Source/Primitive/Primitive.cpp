#include "PCH.h"
#include "Shapes/Sphere.h"
#include "Shapes/TriangleMesh.h"
#include "Math/Ray.h"
#include "Primitive.h"

Primitive::Primitive()
{

}

Primitive::~Primitive()
{

}

void Primitive::SetShape( Shape* _shape )
{
	shapes.push_back( _shape );
}

bool Primitive::Intersect( Ray& r , IntersectRecord* record ) const
{
	record->primitivePtr = this;

	bool bHit = false;

	for( int i = 0; i < shapes.size(); i++ )
	{
		if( shapes[i]->Intersect( r , record ) )
		{
			record->HitPointColor = color;

			bHit = true;
		}
	}
	
	return bHit;
}

bool Primitive::IntersectP( const Ray& r ) const
{
	bool bHit = false;

	for( int i = 0; i < shapes.size(); i++ )
	{
		if( shapes[i]->IntersectP( r ) )
		{
			bHit = true;

			break;					// 因为只需要判断是否有相交，所以出现相交直接退出即可，无需找到最近点
		}
	}

	return bHit;
}

void Primitive::SetDiffuseColor( const Spectrum& _color )
{
	color = _color;
}

void Primitive::ParsePrimitive( XMLElement* PrimitiveRootElment )
{
	XMLElement* PrimitiveShapeElement = PrimitiveRootElment->FirstChildElement( "shape" );

	const char* ShapeType = PrimitiveShapeElement->FirstAttribute()->Value();

	if( !std::strcmp( "sphere" , ShapeType ) )
	{
		Sphere* shape = new Sphere;
		shapes.push_back( shape );
		shape->ParseShape( PrimitiveShapeElement );
	}
	else if( !std::strcmp( "obj" , ShapeType ) )
	{
		TriangleMesh* mesh = new TriangleMesh;
		shapes.push_back( mesh );
		mesh->ParseShape( PrimitiveShapeElement );
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