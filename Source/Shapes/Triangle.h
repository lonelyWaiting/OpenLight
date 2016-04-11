#pragma once

#include "Math/Bound3.h"
#include "Math/Point3.h"
#include "Shape.h"

class Triangle
{
public:
	Triangle();

	Triangle( int _index0 , int _index1 , int _index2 );

	~Triangle();

	void Set( int _index0 , int _index1 , int _index2 );

	bool Intersect( const Ray& ray , const Point3f* points , IntersectRecord* record ) const;

	bool IntersectP( const Ray& ray , const Point3f* points ) const;

private:
	int index0 , index1 , index2;
};