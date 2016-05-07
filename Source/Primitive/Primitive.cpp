#include "Utilities/PCH.h"
#include "Shapes/Sphere.h"
#include "Shapes/TriangleMesh.h"
#include "Math/Ray.h"
#include "Material/GlassMaterial.h"
#include "Material/DiffuseMaterial.h"
#include "Material/PureReflectionMaterial.h"
#include "Light/AreaLighth.h"
#include "tinyxml2.h"
#include "Primitive.h"

Primitive::Primitive()
{
	m_pMaterial = nullptr;
	m_pAreaLight = nullptr;
	m_SumArea = 0.0;
}

Primitive::~Primitive()
{
	
}

void Primitive::AddShape( Shape* _shape )
{
	_shape->SetPrimitive( this );

	m_vShapes.push_back( _shape );

	m_SumArea += _shape->Area();

	BBoxLocal.ExpendToInclude( _shape->GetObjectBoundingBox() );
	BBoxWorld.ExpendToInclude( _shape->GetWorldBoundingBox() );
}

bool Primitive::Intersect( Ray& r , IntersectRecord* record ) const
{
	if( BBoxWorld.IntersectP( r ) )
	{
		record->primitivePtr = this;

		bool bHit = false;

		for( unsigned int i = 0; i < m_vShapes.size(); i++ )
		{
			if( m_vShapes[i]->Intersect( r , record ) )
			{
				record->HitPoint = r( record->HitT );
				bHit = true;
			}
		}

		return bHit;
	}

	return false;
}

void Primitive::SetMaterial(Material* material)
{
	m_pMaterial = material;
}

void Primitive::Deserialization( tinyxml2::XMLElement* PrimitiveRootElment )
{
	tinyxml2::XMLElement* ShapeRootElement = PrimitiveRootElment->FirstChildElement( "shape" );

	while( ShapeRootElement )
	{
		DeserializationShape( ShapeRootElement );

		ShapeRootElement = ShapeRootElement->NextSiblingElement( "shape" );
	}
	
	// least one material node
	tinyxml2::XMLElement* MaterialRootElement = PrimitiveRootElment->FirstChildElement( "material" );
	DeserializationMaterial( MaterialRootElement );

	// AreaLight
	tinyxml2::XMLElement* AreaLightRootElement = PrimitiveRootElment->FirstChildElement( "AreaLight" );
	DeserializationAreaLight( AreaLightRootElement );

	// Update Bounding Box
	for( unsigned int i = 0; i < m_vShapes.size(); i++ )
	{
		BBoxLocal.ExpendToInclude( m_vShapes[i]->GetObjectBoundingBox() );
		BBoxWorld.ExpendToInclude( m_vShapes[i]->GetWorldBoundingBox() );
	}
}

void Primitive::DeserializationShape( tinyxml2::XMLElement* ShapeRootElement )
{
	const char* ShapeType = ShapeRootElement->FirstAttribute()->Value();
	const char* bCompositeObject = ShapeRootElement->Attribute( "bCompositeObject" );
	
	Shape* shape = Shape::Create( ShapeType );
	Assert( shape != nullptr );

	shape->Deserialization( ShapeRootElement );

	if( !std::strcmp( bCompositeObject , "true" ) )
	{
		// 返回一系列的Shape对象
		for( int i = 0; i < shape->GetChildCount(); i++ )
		{
			AddShape( shape->GetChild( i ) );
		}
	}
	else
	{
		AddShape( shape );
	}
}

void Primitive::DeserializationMaterial( tinyxml2::XMLElement* MaterialRootElement )
{
	Assert( MaterialRootElement != nullptr );

	const char* MaterialType = MaterialRootElement->FirstAttribute()->Value();

	m_pMaterial = Material::Create( MaterialType );
	Assert( m_pMaterial != nullptr );
	m_pMaterial->Deserialization( MaterialRootElement );
}

void Primitive::DeserializationAreaLight( tinyxml2::XMLElement* AreaLightRootElement )
{
	if( AreaLightRootElement )
	{
		m_pAreaLight = new AreaLight;
		m_pAreaLight->Deserialization( AreaLightRootElement );
		m_pAreaLight->SetPrimitive( this );
	}
}

BSDF* Primitive::GetBSDF( const Point3f& point , const Normal& normal ) const
{
	return m_pMaterial->GetBSDF( point , normal );
}

int Primitive::GetShapeCount() const
{
	return m_vShapes.size();
}

Shape* Primitive::GetShape( int index )
{
	return m_vShapes[index];
}

void Primitive::AddAreaLight( AreaLight* _pAreaLight )
{
	m_pAreaLight = _pAreaLight;
}

double Primitive::PDF( const Point3f& p , const Vector3f& wi )
{
	double pdf = 0.0;

	for( unsigned int i = 0; i < m_vShapes.size(); i++ )
	{
		pdf += m_vShapes[i]->Area() * m_vShapes[i]->PDF( p , wi );
	}
	
	return pdf / m_SumArea;
}

double Primitive::GetArea() const
{
	return m_SumArea;
}

Light* Primitive::GetAreaLight() const
{ 
	return m_pAreaLight;
}