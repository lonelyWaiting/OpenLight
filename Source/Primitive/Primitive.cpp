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
}

bool Primitive::Intersect( Ray& r , IntersectRecord* record ) const
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

void Primitive::SetMaterial(Material* material)
{
	pMaterial = material;
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

	while (PrimitiveShapeElement = PrimitiveShapeElement->NextSiblingElement("shape"))
	{
		ShapeType = PrimitiveShapeElement->FirstAttribute()->Value();

		if (!std::strcmp("sphere", ShapeType))
		{
			Sphere* shape = new Sphere;
			shapes.push_back(shape);
			shape->ParseShape(PrimitiveShapeElement);
		}
		else if (!std::strcmp("obj", ShapeType))
		{
			TriangleMesh* mesh = new TriangleMesh;
			shapes.push_back(mesh);
			mesh->ParseShape(PrimitiveShapeElement);
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
}

BxDF* Primitive::GetBxDF( const Point3f& point , const Normal& normal ) const
{
	return pMaterial->GetBxDF( point , normal );
}