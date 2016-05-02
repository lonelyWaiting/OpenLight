#pragma once

#include "PCH.h"
#include "Math/Bound3.h"
#include "IntersectRecord.h"
#include "Shapes/Shape.h"
#include "Spectrum/Spectrum.h"
#include "Material/Material.h"
#include "Core/VSerializableObject.h"
#include "Core/VIntersectable.h"

class AreaLight;
class Light;

class Primitive : public VSerializableObject , public VIntersectable
{
public:
	Primitive();

	virtual ~Primitive();

	virtual bool Intersect( Ray& r , IntersectRecord* record ) const;

	void AddShape( Shape* _shape );

	Shape* GetShape( int index );

	int GetShapeCount() const;

	BSDF* GetBSDF( const Point3f& point , const Normal& normal ) const;

	void SetMaterial(Material* material);

	void AddAreaLight( AreaLight* _pAreaLight );

	Light* GetAreaLight() const;

public:
	void Deserialization( XMLElement* PrimitiveRootElment );

	void DeserializationShape( XMLElement* ShapeRootElement );

	void DeserializationMaterial( XMLElement* MaterialRootElement );

	void DeserializationAreaLight( XMLElement* AreaLightRootElement );

public:
	double PDF( const Point3f& p , const Vector3f& w );

public:
	double GetArea() const;

protected:
	std::vector<Shape*> m_vShapes;
	Material*           m_pMaterial;
	AreaLight*          m_pAreaLight;
	double              m_SumArea;
};