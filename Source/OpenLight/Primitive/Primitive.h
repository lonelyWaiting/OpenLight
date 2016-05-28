#pragma once

#include "Utilities/PCH.h"
#include "Math/Bound3.h"
#include "IntersectRecord.h"
#include "Shapes/Shape.h"
#include "Spectrum/Spectrum.h"
#include "Material/Material.h"
#include "Core/VSerializableObject.h"
#include "Core/VIntersectable.h"

class AreaLight;
class Light;

namespace tinyxml2
{
	class XMLDocument;
	class XMLElement;
}

class Primitive : public VSerializableObject , public VIntersectable
{
public:
	Primitive();

	virtual ~Primitive();

	virtual bool Intersect( Ray& r , IntersectRecord* record ) const;

	void AddShape( Shape* _shape );

	Shape* GetShape( int index ) const;

	int GetShapeCount() const;

	BSDF* GetBSDF( const Point3f& point , const Normal& normal ) const;

	void SetMaterial(Material* material);

	void AddAreaLight( AreaLight* _pAreaLight );

	Light* GetAreaLight() const;

public:
	void Deserialization( tinyxml2::XMLElement* PrimitiveRootElment );

	void DeserializationShape( tinyxml2::XMLElement* ShapeRootElement );

	void DeserializationMaterial( tinyxml2::XMLElement* MaterialRootElement );

	void DeserializationAreaLight( tinyxml2::XMLElement* AreaLightRootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );

public:
	double PDF( const Point3f& p , const Vector3f& w );

public:
	double GetArea() const;

public:
	const char* GetName() const;

	void SetName( const char* name );

protected:
	std::vector<Shape*>           m_vShapes;
	Material*                     m_pMaterial;
	AreaLight*                    m_pAreaLight;
	double                        m_SumArea;

	std::vector<Shape*>           m_vShapeInformations;

	char*					  m_Name;
};