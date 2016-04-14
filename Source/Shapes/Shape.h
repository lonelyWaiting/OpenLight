#pragma once

#include "Math/Bound3.h"
#include "Spectrum\Spectrum.h"

class Transform;

class Ray;

struct IntersectRecord;

class Shape
{
public:
	Shape(Spectrum _emmisive = Spectrum(0));

	Shape( const Transform* ObjectToWorld , const Transform* WorldToObject );

	virtual ~Shape();

	virtual Bound3f ObjectBound() const = 0;
	
	virtual Bound3f WorldBound() const;

	virtual bool Intersect( Ray& ray , IntersectRecord* record ) const;

	virtual bool IntersectP( const Ray& ray ) const;

	virtual void ParseShape( XMLElement* ShapeRootElement ) = 0;

public:
	Transform*	ObjectToWorld;
	Transform*	WorldToObject;
	Spectrum	emmisive;
};