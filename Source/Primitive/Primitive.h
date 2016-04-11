#pragma once

#include "PCH.h"
#include "Math/Bound3.h"
#include "IntersectRecord.h"
#include "Shapes/Shape.h"
#include "Spectrum/Spectrum.h"

class Primitive
{
public:
	Primitive();

	virtual ~Primitive();

	virtual bool Intersect( Ray& r , IntersectRecord* record ) const;

	virtual bool IntersectP( const Ray& r ) const;

	void SetShape( Shape* _shape );

	void SetDiffuseColor( const Spectrum& _color );

public:
	void ParsePrimitive( XMLElement* PrimitiveRootElment );

protected:
	Spectrum color;

	std::vector<Shape*> shapes;
};