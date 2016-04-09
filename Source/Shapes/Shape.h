#pragma once

#include "Math/Bound3.h"

class Transform;

class Ray;

struct IntersectRecord;

class Shape
{
public:
	Shape( const Transform* ObjectToWorld , const Transform* WorldToObject );

	virtual ~Shape();

	virtual Bound3f ObjectBound() const = 0;
	
	virtual Bound3f WorldBound() const;

	virtual bool Intersect( const Ray& ray , IntersectRecord* record ) const;

	virtual bool IntersectP( const Ray& ray ) const;

public:
	Transform*	ObjectToWorld;
	Transform*	WorldToObject;
};