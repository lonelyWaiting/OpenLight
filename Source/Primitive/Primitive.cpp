#include "PCH.h"
#include "Shapes/Sphere.h"
#include "Shapes/TriangleMesh.h"
#include "Math/Ray.h"
#include "Material/DiffuseMaterial.h"
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
				record->Emmisive = shapes[i]->emmisive;
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
	XMLElement* PrimitiveShapeElement = PrimitiveRootElment->FirstChildElement( "shape" );

	const char* ShapeType = PrimitiveShapeElement->FirstAttribute()->Value();

	if( !std::strcmp( "sphere" , ShapeType ) )
	{
		Sphere* shape = new Sphere;
		shapes.push_back( shape );
		shape->SetPrimitive( this );
		shape->Deserialization( PrimitiveShapeElement );
	}
	else if( !std::strcmp( "obj" , ShapeType ) )
	{
		TriangleMesh* mesh = new TriangleMesh;
		shapes.push_back( mesh );
		mesh->SetPrimitive( this );
		mesh->Deserialization( PrimitiveShapeElement );
	}
	else
	{
		Assert( "don't support \'%s\' shape object" , ShapeType );
	}

	while (PrimitiveShapeElement = PrimitiveShapeElement->NextSiblingElement("shape"))
	{
		ShapeType = PrimitiveShapeElement->FirstAttribute()->Value();

		if (!std::strcmp("sphere", ShapeType))
		{
			Sphere* shape = new Sphere;
			shapes.push_back(shape);
			shape->SetPrimitive( this );
			shape->Deserialization(PrimitiveShapeElement);
		}
		else if (!std::strcmp("obj", ShapeType))
		{
			TriangleMesh* mesh = new TriangleMesh;
			shapes.push_back(mesh);
			mesh->SetPrimitive( this );
			mesh->Deserialization(PrimitiveShapeElement);
		}
		else
		{
			Assert("don't support \'%s\' shape object", ShapeType);
		}
	}

	
	XMLElement* MaterialElement = PrimitiveRootElment->FirstChildElement( "material" );
	const char* MaterialType = MaterialElement->FirstAttribute()->Value();

	if (!std::strcmp("diffuse", MaterialType))
	{
		XMLElement* MaterialReflectElement = MaterialElement->FirstChildElement("Reflection");
		
		double r, g, b;
		MaterialReflectElement->FirstChildElement("r")->QueryDoubleText(&r);
		MaterialReflectElement->FirstChildElement("g")->QueryDoubleText(&g);
		MaterialReflectElement->FirstChildElement("b")->QueryDoubleText(&b);

		pMaterial = new DiffuseMaterial(Spectrum::FromRGB(r, g, b));
	}

	for( int i = 0; i < shapes.size(); i++ )
	{
		BBoxLocal.ExpendToInclude( shapes[i]->GetObjectBoundingBox() );
		BBoxWorld.ExpendToInclude( shapes[i]->GetWorldBoundingBox() );
	}
}

BxDF* Primitive::GetBxDF( const Point3f& point , const Normal& normal ) const
{
	return pMaterial->GetBxDF( point , normal );
}

int Primitive::GetShapeCount() const
{
	return shapes.size();
}

Shape* Primitive::GetShape( int index )
{
	return shapes[index];
}