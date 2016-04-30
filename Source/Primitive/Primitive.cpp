#include "PCH.h"
#include "Shapes/Sphere.h"
#include "Shapes/TriangleMesh.h"
#include "Math/Ray.h"
#include "Material/GlassMaterial.h"
#include "Material/DiffuseMaterial.h"
#include "Primitive.h"

Primitive::Primitive()
{

}

Primitive::~Primitive()
{

}

void Primitive::AddShape( Shape* _shape )
{
	shapes.push_back( _shape );

	BBoxLocal.ExpendToInclude( _shape->GetObjectBoundingBox() );
	BBoxWorld.ExpendToInclude( _shape->GetWorldBoundingBox() );
}

bool Primitive::Intersect( Ray& r , IntersectRecord* record ) const
{
	if( BBoxWorld.IntersectP( r ) )
	{
		record->primitivePtr = this;

		bool bHit = false;

		for( int i = 0; i < shapes.size(); i++ )
		{
			if( shapes[i]->Intersect( r , record ) )
			{
				record->HitPoint = r( record->HitT );
				record->Emission = shapes[i]->Emissive;
				bHit = true;
			}
		}

		return bHit;
	}

	return false;
}

void Primitive::SetMaterial(Material* material)
{
	pMaterial = material;
}

void Primitive::Deserialization( XMLElement* PrimitiveRootElment )
{
	XMLElement* ShapeRootElement = PrimitiveRootElment->FirstChildElement( "shape" );

	while( ShapeRootElement )
	{
		DeserializationShape( ShapeRootElement );

		ShapeRootElement = ShapeRootElement->NextSiblingElement( "shape" );
	}
	
	// least one material node
	XMLElement* MaterialRootElement = PrimitiveRootElment->FirstChildElement( "material" );
	DeserializationMaterial( MaterialRootElement );

	// Update Bounding Box
	for( int i = 0; i < shapes.size(); i++ )
	{
		BBoxLocal.ExpendToInclude( shapes[i]->GetObjectBoundingBox() );
		BBoxWorld.ExpendToInclude( shapes[i]->GetWorldBoundingBox() );
	}
}

void Primitive::DeserializationShape( XMLElement* ShapeRootElement )
{
	const char* ShapeType = ShapeRootElement->FirstAttribute()->Value();

	Shape* shape = Shape::Create( ShapeType );
	Assert( shape != nullptr );
	shapes.push_back( shape );
	shape->SetPrimitive( this );
	shape->Deserialization( ShapeRootElement );
}

void Primitive::DeserializationMaterial( XMLElement* MaterialRootElement )
{
	Assert( MaterialRootElement != nullptr );

	const char* MaterialType = MaterialRootElement->FirstAttribute()->Value();

	pMaterial = Material::Create( MaterialType );
	Assert( pMaterial != nullptr );
	pMaterial->Deserialization( MaterialRootElement );
}

BSDF* Primitive::GetBSDF( const Point3f& point , const Normal& normal ) const
{
	return pMaterial->GetBSDF( point , normal );
}

int Primitive::GetShapeCount() const
{
	return shapes.size();
}

Shape* Primitive::GetShape( int index )
{
	return shapes[index];
}