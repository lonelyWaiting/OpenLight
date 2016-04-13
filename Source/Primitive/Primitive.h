#pragma once

#include "PCH.h"
#include "Math/Bound3.h"
#include "IntersectRecord.h"
#include "Shapes/Shape.h"
#include "Spectrum/Spectrum.h"
#include "Material/Material.h"

class Primitive
{
public:
	Primitive();

	virtual ~Primitive();

	virtual bool Intersect( Ray& r , IntersectRecord* record ) const;

	virtual bool IntersectP( const Ray& r ) const;

	void SetShape( Shape* _shape );

	BxDF* GetBxDF( const Point3f& point , const Normal& normal ) const;

	void SetMaterial(Material* material);

public:
	void ParsePrimitive( XMLElement* PrimitiveRootElment );

protected:
	std::vector<Shape*> shapes;

	Material* pMaterial;
};