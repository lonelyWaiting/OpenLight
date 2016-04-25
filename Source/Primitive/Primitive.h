#pragma once

#include "PCH.h"
#include "Math/Bound3.h"
#include "IntersectRecord.h"
#include "Shapes/Shape.h"
#include "Spectrum/Spectrum.h"
#include "Material/Material.h"
#include "Core/VSerializableObject.h"
#include "Core/VIntersectable.h"

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

public:
	void Deserialization( XMLElement* PrimitiveRootElment );

	void DeserializationShape( XMLElement* ShapeRootElement );

	void DeserializationMaterial( XMLElement* MaterialRootElement );

protected:
	std::vector<Shape*> shapes;

	Material* pMaterial;
};