#pragma once

#include "Utilities/PCH.h"
#include "Math/Bound3.h"
#include "IntersectRecord.h"
#include "Shapes/Shape.h"
#include "Spectrum/Spectrum.h"
#include "Material/Material.h"
#include "Core/VSerializableObject.h"
#include "Core/VIntersectable.h"
#include "Math/MathHelper.h"

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

	virtual bool Intersect( Rayf& r , IntersectRecord* record ) const;

	void AddShape( Shape* _shape );

	// 获取一个Shape：如Triangle,Sphere这些最基本的图元
	Shape* GetShape( int index ) const;

	int GetShapeCount() const;

	BSDF* GetBSDF( const Vector2f& uv , const Point3f& point , const Vector3f& normal ) const;

	Material* GetMaterial() const;

	void SetMaterial(Material* material);

	void AddAreaLight( AreaLight* _pAreaLight );

	Light* GetAreaLight() const;

	// 获取物体对象，比如一个球，或者一个TriangeMesh而Triangle
	Shape* GetPrimitiveObject( int index ) const;

	Vector2f GetUVScale() const;

public:
	void Deserialization( tinyxml2::XMLElement* PrimitiveRootElment );

	void DeserializationShape( tinyxml2::XMLElement* ShapeRootElement );

	void DeserializationMaterial( tinyxml2::XMLElement* MaterialRootElement );

	void DeserializationAreaLight( tinyxml2::XMLElement* AreaLightRootElement );

	void Serialization( tinyxml2::XMLDocument& xmlDoc , tinyxml2::XMLElement* pRootElement );

public:
	float PDF( const Point3f& p , const Vector3f& w );

public:
	float GetArea() const;

public:
	const char* GetName() const;

	void SetName( const char* name );

protected:
	std::vector<Shape*>				m_vShapes;
	Material*						m_pMaterial;
	AreaLight*						m_pAreaLight;
	float							m_SumArea;

	std::vector<Shape*>				m_vShapeInformations;

	char*							m_Name;

	Vector2f						mTexScale;
};